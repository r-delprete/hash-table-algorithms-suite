#include "./include/hash_table.hpp"

using namespace std;

int main(int argc, char** argv) {
  ifstream input("input.txt");
  ofstream output("output.txt");
  ifstream exam_input("exam_input_example.txt");

  HashTable<int, string> ht(10, input, Hashing::multiply);
  HashTable<int, string> ht_exam(15, exam_input);

  ht.print();
  ht_exam.print(cout, "Exam hash table");
  ht.print(output, "Hash table");
  ht_exam.print(output, "Exam hash table");

  ht.search(6);
  ht.delete_item(12);

  cout << endl;
  ht.print(cout, "Base hash table after deleting item 12");

  input.close();
  output.close();
  exam_input.close();

  return 0;
}