#include "log.h"
#include "parser.h"
#include "parser.tab.h"
#include "ast.h"
#include <fstream>
#include <algorithm>
#include <iostream> // 新增，用于打印帮助信息和错误
#include <string>   // 新增，确保 string 可用
#include <vector>   // 新增，用于存储参数
#include <stdexcept> // 新增，用于 stoi 错误处理
#include <set>
#include <chrono>
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using namespace std::chrono;
extern FILE* yyin;
const std::string stdout_ = "";
const std::string stdin_ = "";

void yyinput(const char* in);
void yydebug_(int level);


int CompilePascalFile(std::string in_file_path, std::string out_file_path = stdout_);
int GenerateCodeFromAST(AST* ast_tree, std::string out_file_path);
void ResetCompilationTimer();
double GetCompilationDurationMs();
void print_help() {
    cout << "usage: pascc [-i inputfile] [-o outputfile] [-d debuglevel] [-h|--help]" << endl;
    cout << "optional arguments:" << endl;
    cout << " -h, --help      show help message" << endl;
    cout << " -i, --input     input pascal-s file (default: test.pas)" << endl;
    cout << " -o, --output    output c file (default: <input_name_without_ext>.c)" << endl;
    cout << " -d, --debug     debug level (0: QUIET, 1: ERROR, 2: WARN, 3: INFO, 4: DEBUG, 5: TRACE, default: 3)" << endl;
}

int main(int argc, char** argv) {
  string input_file = "test.pas";
  string output_file = "";
  int debug_level = 3;
  bool help_flag = false;

  for (int i = 1; i < argc; ++i) {
    string arg = argv[i];
    if (arg == "-h" || arg == "--help") {
      help_flag = true;
      break;
    } else if (arg == "-i" || arg == "--input") {
      if (i + 1 < argc) {
        input_file = argv[++i];
      } else {
        cerr << "Error: -i/--input option requires an argument." << endl;
        return 1;
      }
    } else if (arg == "-o" || arg == "--output") {
      if (i + 1 < argc) {
        output_file = argv[++i];
      } else {
        cerr << "Error: -o/--output option requires an argument." << endl;
        return 1;
      }
    } else if (arg == "-d" || arg == "--debug") {
      if (i + 1 < argc) {
        try {
          debug_level = std::stoi(argv[++i]);
          if (debug_level < 0 || debug_level > 5) {
            cerr << "错误: 调试级别必须在0和5之间." << endl;
            return 1;
          }
        } catch (const std::invalid_argument& ia) {
          cerr << "错误: 调试级别必须是整数." << endl;
          return 1;
        } catch (const std::out_of_range& oor) {
          cerr << "错误: 调试级别超出范围." << endl;
          return 1;
        }
      } else {
        cerr << "错误: -d/--debug选项需要一个参数." << endl;
        return 1;
      }
    } else {
      cerr << "错误: 未知选项 '" << arg << "'." << endl;
      print_help();
      return 1;
    }
  }

  if (help_flag) {
    print_help();
    return 0;
  }

  // set debug level
  log_set_level(5 - debug_level);
  yydebug_(debug_level);


  // get input source file
  string in = input_file;
  // get output destination
  string out = output_file;
  if (output_file.empty()) {
    // 如果没有指定输出文件，则生成与输入文件同名的.c文件
    // 查找输入文件的扩展名位置
    size_t dot_pos = in.find_last_of('.');
    if (dot_pos != string::npos) {
      // 提取不含扩展名的部分，然后添加.c扩展名
      out = in.substr(0, dot_pos) + ".c";
    } else {
      // 如果输入文件没有扩展名，直接添加.c
      out = in + ".c";
    }
  }

  size_t out_len = out.length();
  if (out_len > 2) {
    if (out[out_len - 1] == 'c' && out[out_len - 2] == '.') {
      out = out.substr(0, out_len - 2);
    }
  }

  if (CompilePascalFile(in, out) < 0) return 1;

  return 0;
}

static std::chrono::high_resolution_clock::time_point g_compilation_start_point;

int CompilePascalFile(string in_file_path, string out_file_path /*, string st*/) {
  ResetCompilationTimer();
  yyinput(in_file_path.length() == 0 ? nullptr : in_file_path.c_str());
  AST ast;
  try {
    yyparse(&ast);
  } catch (std::exception& e) {
    log_error("内部错误: %s", e.what());
    return -1;
  }

  if (ast.Valid()) {
    log_info("tick():词法/语法分析完成，耗时 %lf ms.", GetCompilationDurationMs());
    log_info("成功生成AST.");
  } else {
    log_error("词法/语法分析失败.");
    return -1;
  }

  // ast format
  return GenerateCodeFromAST(&ast, out_file_path /*, st*/);
}

int GenerateCodeFromAST(AST* ast_tree, string out_file_path /*, string st*/) {
  if (ast_tree == nullptr || ast_tree->root() == nullptr) return -1;
  FILE* dst = stdout;
  bool file_opened = false;
  if (out_file_path.length() > 0) {
    dst = fopen((out_file_path + ".c").c_str(), "w");
    if (dst != nullptr) {
        file_opened = true;
    } else {
        // If fopen failed but we specified a path, dst might be null.
        // It was already handled by the next check, but good to be explicit.
    }
  }

  if (dst == nullptr) { // This handles fopen failure
    log_fatal("失败:无法打开文件 %s.c 用于写入", out_file_path.c_str());
    return -1;
  }
  ResetCompilationTimer();
  CodeGenerator::FormatProgram(ast_tree, dst);
  log_info("tick(): 格式化AST完成，耗时 %lf ms.", GetCompilationDurationMs());
  
  if (file_opened) { // Only close if we opened it.
    fclose(dst);
  }
  return 0;
}

void ResetCompilationTimer() {
  g_compilation_start_point = std::chrono::high_resolution_clock::now();
}

double GetCompilationDurationMs() {
  auto cur = std::chrono::high_resolution_clock::now();
  duration<double,std::ratio<1,1000>> duration_ms=duration_cast<duration<double,std::ratio<1,1000>>>(cur - g_compilation_start_point);
  return duration_ms.count();
}

void yyinput(const char* in) {
  if (in == nullptr) {
    yyin = stdin;
    log_info("开始编译...");
    return;
  }

  log_info("开始编译: %s", in);
  yyin = fopen(in, "r");
  if (yyin == nullptr)
    throw std::runtime_error("yyinput(): failed to open file " + string(in));
}

void yydebug_(int level) {
  // TODO  set yydebug
  yydebug = (level == 5);
}
