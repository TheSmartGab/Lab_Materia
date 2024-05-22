#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <regex>

#include "fmtlib.h"

#define DEBUG
#define DEBUG_1
//#define RUN_2
//#define FILE_NAMES
#define DATA_NAMES

class file_name
{
  public:
  file_name () {} //constructor without arguments
  file_name (std::string in_file_name) //constructor with arguments
  {
    std::stringstream tmp_holder; 
    tmp_holder << in_file_name;
    //
    std::string tmp_date;
    getline (tmp_holder, tmp_date, '_');
    m_date = tmp_date + "_";
    tmp_date.erase();   
    getline (tmp_holder, tmp_date, '_');
    m_date += tmp_date + "_";
    tmp_date.erase();
    getline (tmp_holder, tmp_date, '_');
    m_date += tmp_date;
    //
    getline (tmp_holder, m_test_beam, '_');
    //
    getline (tmp_holder, m_beam, '_');
    //
    getline (tmp_holder, m_id, '_');
    //
    getline (tmp_holder, m_data_taking_mode, '_');
    //
    std::string tmp_lambda;
    getline (tmp_holder, tmp_lambda, '_');
    m_lambda = std::stoi(tmp_lambda);
    //
    std::string tmp_abs;
    getline (tmp_holder, tmp_abs, '_');
    m_abs = std::stod(tmp_abs);
    #ifdef DEBUG
      std::cerr << "\nAbs (unit)\tloaded: " << tmp_abs << "\tsaved: " << m_abs << std::endl;
    #endif
    tmp_abs.erase();
    getline (tmp_holder, tmp_abs, '_');
    m_abs += (std::stod(tmp_abs)/10000);
    #ifdef DEBUG
      std::cerr << "Abs (decimals)\tloaded: " << tmp_abs << "\tsaved total: " << m_abs << std::endl;
    #endif
    m_abs_sign = m_abs_sign_check (in_file_name);
    #ifdef DEBUG
      std::cerr << "Abs sign: " << m_abs_sign << " (1 = + ; 0 = -)" << std::endl;
    #endif
    //
    std::string tmp_run;
    getline (tmp_holder, tmp_run, '_');
    tmp_run.erase();
    getline (tmp_holder, tmp_run, '_');
    getline (tmp_holder, tmp_run, '_');
    m_run = std::stoi(tmp_run);
  }
  ~file_name () {} //distructor

  void modify_m_date (std::string in_date) {m_date = in_date;}
  void modify_m_test_beam (std::string in_test_beam) {m_test_beam = in_test_beam;}
  void modify_m_id (std::string in_id) {m_id = in_id;}
  void modify_m_data_taking_mode (std::string in_data_taking_mode) {m_data_taking_mode = in_data_taking_mode;}
  void modify_m_lambda (int in_lambda) {m_lambda = in_lambda;}
  void modify_m_abs (double in_abs) {m_abs = in_abs;}
  void modify_m_run (int in_run) {m_run = in_run;}

  std::string call_m_date () {return m_date;}
  std::string call_m_test_beam () {return m_test_beam;}
  std::string call_m_id () {return m_id;}
  std::string call_m_data_taking_mode () {return m_data_taking_mode;}
  int call_m_lambda () {return m_lambda;}
  double call_m_abs () {return m_abs;}
  bool call_m_abs_sign () {return m_abs_sign;}
  int call_m_run () {return m_run;}

  private:
  std::string m_date;
  std::string m_test_beam;
  std::string m_beam;
  std::string m_id;
  std::string m_data_taking_mode;
  int m_lambda;
  double m_abs;
  bool m_abs_sign; //if true abs is positive
  int m_run;
  bool m_abs_sign_check (std::string in_file_name) 
  {
    std::regex abs_regexp("[-]",std::regex_constants::icase);
    if (!std::regex_search (in_file_name, abs_regexp)) return true;
    else return false;
    //std::vector<std::regex> stat_regexp;
    //stat_regexp.push_back(std::regex("\\b(a|A)",std::regex_constants::icase));
    //if (std::regex_search (in_stat, stat_regexp[i]))
  }
};

int main () 
{
  //folder selection
  std::string folder;
  std::cout << "please insert folder name (dd/mm/yy): ";
  std::cin >> folder;

  //copy names of the files in the chosen folder
  std::vector<std::string> file_name_container;
  std::fstream file_with_names;
  #ifdef FILE_NAMES
    std::string name_of_file_with_names = "./Lab_Data/"+ folder +"/file_names.txt";
  #endif
  #ifdef DATA_NAMES
    std::string name_of_file_with_names = "./Lab_Data/"+ folder +"/Data_names.txt";
  #endif
  file_with_names.open(name_of_file_with_names);
  if (!file_with_names.good()) 
  {
    #ifdef DEBUG
      std::cerr << "impossibile aprire il file " << name_of_file_with_names << std::endl;
    #endif
    return 1;
  }
  std::string tmp;
  for (int i = 0;;i++) 
  {
    getline (file_with_names, tmp);
    file_name_container.push_back(tmp);
    #ifdef DEBUG
      std::cerr << "name loaded: " << tmp << "\t name saved: " << file_name_container[i] << std::endl;
    #endif
    if (file_with_names.eof()) break;
  }
  file_with_names.close();

  //modify file to add what needed
  auto file_name_container_dim = file_name_container.size();
  bool file_compatible = true;
  for (int i = 0; i < file_name_container_dim; i++) 
  {
    std::string current_file_name = fmt::format("./Lab_Data/{0}/{1}", folder, file_name_container[i]);
    file_name current_file(file_name_container[i]);
    #ifdef RUN_2
      if (current_file.call_m_run() != 2) continue;
    #endif
    #ifdef DEBUG
      std::cerr << "current file name: <" << current_file_name << ">" << std::endl;
    #endif
    
    std::fstream file(current_file_name);
    if (!file.good()) 
    {
      #ifdef DEBUG
        std::cerr << "impossibile aprire il file " << current_file_name << std::endl;
      #endif
      return 1;
    }
    std::stringstream file_holder;
    static const int line = 74;
    std::string tmp_1;
    for (int j = 0; ; j++) 
    {
      getline (file, tmp_1);
      tmp_1.pop_back();
      #ifdef DEBUG_1
        std::cerr << tmp_1;
        std::cerr << "\n line: " << j << " is: <" << tmp_1  << ">" << std::endl;
      #endif
      file_holder << tmp_1 << std::endl;
      if (file.eof()) break;
      else if (j == line) 
      {
        std::string line_74 = "0.000";
        if (tmp_1 != line_74)
        {
          #ifdef DEBUG
            std::cerr << "the file <" << current_file_name << "> is not compatible with the program\n";
            std::cerr << "line " << line << ": <" << tmp_1 << ">\n";
          #endif
          file_compatible = false;
          break;
        }
        else 
        {
          file_holder << "\nLambda:\t" << current_file.call_m_lambda() << "\n"
                      << "Abs:\t\t";
          if (!current_file.call_m_abs_sign()) file_holder << "-";
          file_holder << current_file.call_m_abs() << "\n"
                      << "ID:\t\t\t" << current_file.call_m_id() << "\n"
                    << "Run:\t\t" << current_file.call_m_run() << "\n#\n\n";
        }
      }
      else if (j == line+1) 
      {
        std::string line_75_1 = "\n"; //possible non compatible element
        std::string line_75_2 = "#"; //possible non compatible element
        if ((tmp_1 == line_75_1) || (tmp_1 == line_75_2))
        {
          #ifdef DEBUG
            std::cerr << "the file <" << current_file_name << "> is not compatible with the program\n";
            std::cerr << "line " << line << ": <" << tmp_1 << ">\n";
          #endif
          file_compatible = false;
          break;
        }
      }
    }
    //adding things into the file
    file.close();
    if (file_compatible)
    {
      std::string new_folder_name = fmt::format("Run_{0}",current_file.call_m_run());
      #ifdef DEBUG
        std::cerr << "new folder name is: " << new_folder_name << std::endl;
      #endif
      std::string changed_file_name = fmt::format("./Run_Data/{0}/{1}", new_folder_name, file_name_container[i]);
      std::ofstream changed_file(changed_file_name);
      changed_file << file_holder.str();
    }
  }
  return 0;
}