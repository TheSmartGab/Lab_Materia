#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <regex>
#include <cstdio>   // For remove function
#include <cerrno>   // For errno
#include <cstring>  // For strerror

#include "fmtlib.h"

#define DEBUG
//#define DEBUG_STOI
//#define DEBUG_SSTREAM
//#define DEBUG_COOKED
//#define DEBUG_BIAS
//#define FILE_NAMES
#define FILES_FROM_RUN
#define FILES_TO_TMP
//#define FILE_CHECK

//source ./Run.sh Run_2

class file_name
{
  public:
  file_name () {} //constructor without arguments
  file_name (std::string in_file_name) //constructorn with arguments
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
    #ifdef DEBUG_STOI
      std::cerr << "\nlambda: " << tmp_lambda << "\n";
    #endif
    m_lambda = std::stoi(tmp_lambda);
    //
    std::string tmp_abs;
    getline (tmp_holder, tmp_abs, '_');
    m_abs = std::stod(tmp_abs);
    #ifdef DEBUG_BIAS
      std::cerr << "\nAbs (unit)\tloaded: " << tmp_abs << "\tsaved: " << m_abs << std::endl;
    #endif
    tmp_abs.erase();
    getline (tmp_holder, tmp_abs, '_');
    m_abs += (std::stod(tmp_abs)/10000);
    #ifdef DEBUG_BIAS
      std::cerr << "Abs (decimals)\tloaded: " << tmp_abs << "\tsaved total: " << m_abs << std::endl;
    #endif
    m_abs_sign = m_abs_sign_check (in_file_name);
    #ifdef DEBUG_BIAS
      std::cerr << "Abs sign: " << m_abs_sign << " (1 = + ; 0 = -)" << std::endl;
    #endif
    //
    std::string tmp_run;
    getline (tmp_holder, tmp_run, '_');
    #ifdef DEBUG_BIAS
      std::cerr << "RERun: " << tmp_run << "\n";
    #endif
    std::string rerun = "RERun";
    std::string rerun_2 = "reRun";
    std::string rerun_3 = "rerun";
    if (tmp_run == rerun) m_rerun = true;
    else if (tmp_run == rerun_2) m_rerun = true;
    else if (tmp_run == rerun_3) m_rerun = true;
    else m_rerun = false;
    #ifdef DEBUG_BIAS
      std::cerr << "RERun: " << m_rerun << " (1 = true ; 0 = false)\n";
    #endif
    tmp_run.erase();
    getline (tmp_holder, tmp_run, '_');
    int pos = tmp_holder.tellg();
    #ifdef DEBUG_STOI
      std::cerr << "\nrun: " << tmp_run << "\n";
    #endif
    m_run = std::stoi(tmp_run);
    //
    std::string tmp_eof_check;
    getline (tmp_holder, tmp_eof_check, '\n');
    tmp_holder.seekg(pos);
    std::string no_temp_eof = "1.txt";
    if (tmp_eof_check != no_temp_eof)
    {
      #ifdef DEBUG_COOKED
        std::cerr << "the opened file has been cooked\n";
      #endif
      std::string tmp_cooked;
      getline (tmp_holder, tmp_cooked, '_');
      if (tmp_cooked == "BC") m_BC = true;
      else m_BC = false;
      #ifdef DEBUG_COOKED
        std::cerr << "C is: " << tmp_cooked << std::endl;
      #endif
      tmp_cooked.erase();
      getline (tmp_holder, tmp_cooked, '_');
      #ifdef DEBUG_COOKED
        std::cerr << "Temp is: " << tmp_cooked << std::endl;
      #endif
      #ifdef DEBUG_STOI
        std::cerr << "\ntemperature: " << tmp_cooked << "\n";
      #endif
      m_temp = std::stoi(tmp_cooked);
    }
    else
    {
      m_temp = 0;
      m_BC = false;
    }
  }
  ~file_name () {} //distructor

  void modify_m_date (std::string in_date) {m_date = in_date;}
  void modify_m_test_beam (std::string in_test_beam) {m_test_beam = in_test_beam;}
  void modify_m_id (std::string in_id) {m_id = in_id;}
  void modify_m_data_taking_mode (std::string in_data_taking_mode) {m_data_taking_mode = in_data_taking_mode;}
  void modify_m_lambda (int in_lambda) {m_lambda = in_lambda;}
  void modify_m_abs (double in_abs) {m_abs = in_abs;}
  void modify_m_run (int in_run) {m_run = in_run;}
  void modify_m_rerun (bool in_rerun) {m_rerun = in_rerun;}
  void modify_m_temp (int in_temp) {m_temp = in_temp;}
  void modify_m_BC (bool in_BC) {m_BC = in_BC;}

  std::string call_m_date () {return m_date;}
  std::string call_m_test_beam () {return m_test_beam;}
  std::string call_m_id () {return m_id;}
  std::string call_m_data_taking_mode () {return m_data_taking_mode;}
  int call_m_lambda () {return m_lambda;}
  double call_m_abs () {return m_abs;}
  bool call_m_abs_sign () {return m_abs_sign;}
  int call_m_run () {return m_run;}
  double call_m_rerun () {return m_rerun;}
  int call_m_temp () {return m_temp;}
  bool call_m_BC () {return m_BC;}

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
  bool m_rerun; //true if it's a RERun
  int m_temp; //temperature at whitch it was cooked
  bool m_BC; //the file has been cooked twice at the same temperature (bicooked)
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
  #ifdef FILE_NAMES
    std::cout << "please insert folder name (dd/mm/yy): ";
  #endif
  #ifdef FILES_FROM_RUN
    std::cout << "please insert Run number\nRun: ";
  #endif
  std::cin >> folder;

  //copy names of the files in the chosen folder
  std::vector<std::string> file_name_container;
  std::fstream file_with_names;
  //path from where this program is located to the Run_Data folder (/Run_Data NOT INCLUDED)
  std::string path_name = "..";
  //
  #ifdef FILE_NAMES
    std::string name_of_file_with_names = path_name + "/Lab_Data/" + folder + "/file_names.txt";
  #endif
  #ifdef FILES_FROM_RUN
    std::string name_of_file_with_names = path_name + "/Run_Data/Run_" + folder + "/Data_names.txt";
  #endif
  file_with_names.open(name_of_file_with_names);
  if (!file_with_names.good()) 
  {
    #ifdef DEBUG
      std::cerr << "impossibile aprire il file " << name_of_file_with_names << std::endl;
    #endif
    return 1;
  }
  #ifdef DEBUG
    std::cerr << "\n\n-------------------------------------------------------------------------------------------------\n\n";
  #endif
  std::string tmp;
  for (int i = 0;;i++) 
  {
    getline (file_with_names, tmp);
    file_name_container.push_back(tmp);
    #ifdef DEBUG
      std::cerr << i+1 << ") name loaded: " << tmp << "\t name saved: " << file_name_container[i] << "\n" << std::endl;
    #endif
    if (file_with_names.eof()) break;
  }
  file_with_names.close();

  //modify file to add what needed
  auto file_name_container_dim = file_name_container.size();
  static const int line = 74;
  #ifdef FILE_CHECK
    bool file_compatible = true;
    std::string line_74 = "0.000";
    std::string line_75_1 = "\n"; //possible non compatible element
    std::string line_75_2 = "#"; //possible non compatible element
    std::string line_75_3 = ""; //possible non compatible element
    std::string line_75_4 = " "; //possible non compatible element
    #ifdef DEBUG
      int compatible_file_count = 0;
      int total_file_count = 0;
    #endif
  #endif

  for (int i = 0; i < file_name_container_dim; i++) 
  {
    #ifdef FILE_NAMES
      std::string current_file_name = fmt::format("{0}/Lab_Data/{1}/{2}", path_name, folder, file_name_container[i]);
    #endif
    #ifdef FILES_FROM_RUN
      std::string current_file_name = fmt::format("{0}/Run_Data/Run_{1}/{2}", path_name, folder, file_name_container[i]);
    #endif
    #ifdef DEBUG
      std::cerr << "\n\n-------------------------------------------------------------------------------------------------\n";
      std::cerr << "\ncurrent file name: <" << current_file_name << ">" << std::endl;
    #endif
    if (file_name_container[i] == "") continue;
    else if (file_name_container[i] == " ") continue;
    else if (file_name_container[i] == "\n") continue;
    file_name current_file(file_name_container[i]);
    
    std::ifstream file(current_file_name);
    if (!file.good()) 
    {
      #ifdef DEBUG
        std::cerr << "impossibile aprire il file " << current_file_name << std::endl;
      #endif
      continue;
    }
    #ifdef FILE_CHECK
      #ifdef DEBUG
        total_file_count ++;
      #endif
    #endif
    std::stringstream file_holder;
    std::string tmp_1;
    for (int j = 0; ; j++) 
    {
      if (file.eof()) break;
      getline (file, tmp_1);
      file_holder << tmp_1 << std::endl;
      #ifdef DEBUG_SSTREAM
        std::cerr << "loaded line: " << j << "\tline: " << tmp_1 << "\n\n";
      #endif
      if (j == line) 
      {
        #ifdef FILE_CHECK
          if (tmp_1 != line_74)
          {
            #ifdef DEBUG
              std::cerr << "the file <" << current_file_name << "> is not compatible with the program\n\n\n\n\n\n";
              std::cerr << "line " << j << ": <\n\n\n" << tmp_1 << "\n\n> instead of <" << line_74 << ">\n\n\n\n\n\n";
            #endif
            file_compatible = false;
            break;
          }
          else
          {
          #ifdef DEBUG
            std::cerr << "the file <" << current_file_name << "> might be compatible with the program\n";
            std::cerr << "line " << line << ": <" << tmp_1 << ">\n";
          #endif
        #endif

        file_holder << "\nLambda:\t" << current_file.call_m_lambda() << "\n"
                    << "Abs:\t\t";
        if (!current_file.call_m_abs_sign()) file_holder << "-";
        file_holder << current_file.call_m_abs() << "\n"
                    << "ID:\t\t\t" << current_file.call_m_id() << "\n";
        if (current_file.call_m_rerun()) file_holder << "RERun:\t" << current_file.call_m_run() << "\n";
        else file_holder << "Run:\t\t" << current_file.call_m_run() << "\n";
        if (current_file.call_m_BC()) file_holder << "BisCuit" << "\n";
        file_holder << "Temp:\t\t" << current_file.call_m_temp() << "\n#\n\n";
        
        #ifdef FILE_CHECK
          }
        #endif
      }
      #ifdef FILE_CHECK
        else if (j == line+1) 
        {
          if ((tmp_1 == line_75_1) || (tmp_1 == line_75_2) || (tmp_1 == line_75_3) || (tmp_1 == line_75_4))
          {
            #ifdef DEBUG
              std::cerr << "the file <" << current_file_name << "> is not compatible with the program\n";
              std::cerr << "line " << j << ": <" << tmp_1 << ">\n";
            #endif
            file_compatible = false;
            break;
          }
          else
          {
            #ifdef DEBUG
              std::cerr << "the file <" << current_file_name << "> is compatible with the program\n";
              std::cerr << "line " << j << ": <" << tmp_1 << ">\n";
              compatible_file_count ++;
            #endif
          }
        }
      #endif
    }

    //adding things into the file
    file.close();
    #ifdef FILE_CHECK
    if (file_compatible)
    {
    #endif
      std::string new_folder_name = fmt::format("Run_{0}",current_file.call_m_run());

      #ifndef FILES_TO_TMP
        // Use global remove function
        std::string changed_file_name = fmt::format("{0}/Run_Data/{1}/{2}", path_name, new_folder_name, file_name_container[i]);
        #ifdef DEBUG
        std::cerr << "changed file name: " << changed_file_name << std::endl;
        #endif
        if (std::remove(changed_file_name.c_str()) == 0) 
        {
          std::cout << "file:" << changed_file_name << " has been deleted" << std::endl;
        } 
        else 
        {
          #ifdef DEBUG
          std::cerr << "Error: Could not delete the file. (" << strerror(errno) << ")" << std::endl;
          #endif
        }
      #endif
      #ifdef FILES_TO_TMP
        std::string new_path_name = "./tmp";
        std::string changed_file_name = fmt::format("{0}/Run_Data/{1}/{2}", new_path_name, new_folder_name, file_name_container[i]);
      #endif
      std::ofstream changed_file(changed_file_name);
      if (!changed_file.good()) 
      {
        #ifdef DEBUG
          std::cerr << "impossibile aprire il file " << changed_file_name << std::endl;
        #endif
        return 1;
      }
      changed_file << file_holder.str();
      changed_file.close();
    #ifdef FILE_CHECK
    }
    #endif
  }
  #ifdef FILE_CHECK
    #ifdef DEBUG
      std::cerr << "\n\n-------------------------------------------------------------------------------------------------\n";
      std::cerr << "\nnumber of compatible files: " << compatible_file_count << "/" << total_file_count << std::endl;
      std::cerr << "\n-------------------------------------------------------------------------------------------------\n\n";
    #endif
  #endif
  return 0;
}