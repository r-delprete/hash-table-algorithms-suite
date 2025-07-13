#include "./include/hash_table.hpp"

using namespace std;

int main(int argc, char** argv) {
  ifstream input("input.txt");
  ifstream input2("exam_input_example.txt");
  ofstream output("output.txt");

  HashTable<int, string> ht(12, input);
  ht.print();
  ht.print(output);

  auto item = ht.search(6);
  if (item) {
    cout << "Item ";
    item->print();
    cout << " successfully found" << endl;
  }

  ht.delete_item(12);
  ht.print(cout, "After delete");
  ht.print(output, "After delete");

  input.close();
  input2.close();
  output.close();

  return 0;
}