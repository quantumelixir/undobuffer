#include "undobuffer.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
  Editor editor;
  editor.Write("I have a blue")
      .Delete(4)
      .Write("red car.")
      .Undo()
      .Undo()
      .Write(" SUV that fits 10!")
      .Delete(3)
      .Write("4.");
  cout << "INITIAL STATE: " << editor.GetState() << endl;
  const vector<string> &history = editor.GetHistory();
  // Skip the last state, which cannot be reached through an undo action.
  for (auto it = history.rbegin() + 1; it != history.rend(); it++) {
    cout << "CURRENT STATE: " << editor.GetState() << endl;
    editor.Undo();
    assert(*it == editor.GetState());
  }
  assert(!editor.CanUndo());
  cout << "FINAL STATE  : " << editor.GetState() << endl;
  return 0;
}
