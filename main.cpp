#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;

struct payroll {
    string name;
    string surname;
    string data;
    int payment = 0;
};
void name_check (std::string &name);
void data_check (std::string &data);
bool overflow ();

int main() {

    string command;
    cout << ": ";
    cin >> command;

    while (command != "list" && command != "add") {
        cout << "Incorrect command. Try again: ";
        cin >> command;
    }


    if (command  == "list") {
        std::ifstream out_file ( "payroll.txt");
        payroll person;

        if (!out_file.is_open ()) {
            cout << "file open error.";
        } else {
            for (int i =0; i < 4 && !out_file.eof();i++){
                std::string buffer;
                out_file >> buffer;
                if (i == 0) {person.name = buffer;}
                if (i == 1) {person.surname = buffer;}
                if (i == 2) {person.data = buffer;}
                try {
                    if (i == 3) {
                        int temp = std::stoi(buffer);
                        person.payment = temp;
                        i = -1;
                        // вывод на экран
                        cout << person.name << " " << person.surname << " " << person.data << " " <<person.payment << endl;
                    }
                }catch (const std::exception& obj)
                {
                    std::cerr << obj.what();
                }

            }
            out_file.close ();
        }

    } else if (command == "add") {
        payroll person;

        //ввод данных
        //Ввод имени
        cout << "Enter name: ";
        cin >> person.name;

        // Проверка имени.
        name_check (person.name);

        // Ввод фамилии
        cout << "Enter surname: ";
        cin >> person.surname;

        //проверка фамилии
        name_check (person.surname);

        //ввод даты
        cout << "Enter data (DD,MM,YYYY): ";
        cin >> person.data;

        // проверка даты
        data_check (person.data);

        //ввод зарплаты
        cout << "Enter payment: ";
        cin >> person.payment;
        while  (overflow ()) {cin >> person.payment;}

        // открываем поток на запись
        std::ofstream in_file ("payroll.txt",ios::app);

        // вывод в файл
        if (!in_file.is_open()){
            cout << "file open error!";
        } else {
            in_file << person.name << " " << person.surname << " " << person.data << " " << person.payment << std::endl;
        }
        in_file.close ();
    }

}
// функция проверки на буквы в имени
bool consist_of_letters (std::string &name){
    for (int i = 0; i < name.length ();i++){
        bool is_letter = (name [i]>= 'A' && name [i] <= 'Z'|| name[i] >= 'a' && name [i] <= 'z');
        if (!is_letter) {;return false; }
    }
    return true;
}

//функция для проверки состоит ли дата из точек и цифер.
bool consist_of_num (std::string &data) {
    int count_point = 0;
    for (int i = 0; i < data.length ();i++){
        bool is_number = isdigit(data [i]);
        bool is_point = (data [i] == '.');
        if (is_point) {count_point ++;}
        if (!is_number && !is_point) {;return false;}
    }
    if (data[2] != '.' || data [5] != '.') {return false;}
    if (count_point != 2) {;return false;}
    return true;
}

// функция проверки имени
void name_check (std::string &name){

    bool size_correct = name.size() < 35 && name.size() > 2;
    bool letters_correct = consist_of_letters(name);

    while (!size_correct || !letters_correct)  {
        // Проверка на длину.
        while (name.size() > 35 || name.size() < 2) {
            if (name.size() > 35) { // если имя больше 35 символов.
                cout << "Name cannot be longer than 35 characters!\nEnter again: ";
                cin >> name;
            }
            if (name.size() < 2) { // если имя меньше 2 символов.
                cout << "Name cannot be less than 2 characters long!\nEnter again: ";
                cin >> name;
            }
        }

        // проверка состоит ли имя из букв.
        while (!consist_of_letters(name)) {

            cout << "Name The name must contain only Latin letters\nEnter again: ";
            cin >> name;
        }
        // проверка условий
        size_correct = name.size() < 35 && name.size() > 2;
        letters_correct = consist_of_letters(name);
    }

    // Если первый символ с маленькой буквы - сделать его с большой.
    if (islower(name[0])){
        name[0] = (char) toupper (name[0]);
    }
}
// определяем параметры для корректности дня по месяцу и году
bool day_correct (int month, int year, int day){
    bool correct_day = false;
    if (month == 02){ // дли февраля определяем високосный год
        if (year% 4 == 0){
            if (year%100 == 0 && year%400 != 0){
                correct_day = day >0 && day <29;
            } else {correct_day = day >0 && day <=29;}
        }
    }else if (month == 04 ||month == 06 || month == 9|| month == 11) {
        correct_day = day > 0 && day <= 30;
    } else { correct_day = day > 0 && day <= 31;}
    return correct_day;
}
// определяем корректность даты
void data_check (std::string &data){
    bool correct_format = consist_of_num (data);

    while (!correct_format){
        cout << "Data the date must contain only numbers and be in the format DD.MM.YYYY. Input again: ";
        cin >> data;
        correct_format = consist_of_num(data);
    }

    int day = stoi (data.substr(0,2));
    int month = stoi (data.substr(3,2));
    int year = stoi (data.substr(6,4));


    bool correct_month = month > 0 && month <= 12;
    bool correct_year = year > 1990 && year < 2022 ;
    bool correct_day = day_correct(month,year,day);

      bool all_correct = correct_format && correct_month && correct_year && correct_day;
      while (!all_correct) {
          while (!correct_format) { // дополнительная проверка если
              cout << "Data the date must contain only numbers and be in the format DD.MM.YYYY. Input again: ";
              cin >> data;
              correct_format = consist_of_num(data);
          }

          while (!correct_month || !correct_day || !correct_year) {

              while (!correct_day) {

                  cout << "invalid value for day. Enter again: ";
                  cin >> data;

                  day = stoi (data.substr(0,2));
                  month = stoi (data.substr(3,2));
                  year = stoi (data.substr(6,4));
                  correct_day = day_correct(month,year,day);
              }
              while (!correct_month){
                  cout << "invalid value for month. Enter again: ";
                  cin >> data;

                  day = stoi (data.substr(0,2));
                  month = stoi (data.substr(3,2));
                  year = stoi (data.substr(6,4));
                  correct_month = month > 0 && month <= 12;
              }
              while (!correct_year){
                  cout << "invalid value for year. Enter again: ";
                  cin >> data;

                  day = stoi (data.substr(0,2));
                  month = stoi (data.substr(3,2));
                  year = stoi (data.substr(6,4));
                  correct_year = year > 1990 && year < 2022;
              }
              correct_format = consist_of_num (data);
          }
          all_correct = correct_format && correct_month && correct_year && correct_day;
      }
}

bool overflow ( ){

    if (! std::cin){
        std::cout <<"Input Error. Enter again: ";
        std::cin.clear();
        std::cin.ignore(1000, '\n');

        return true;
    } else{ return false;
    }
}