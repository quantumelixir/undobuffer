#ifndef UNDOBUFFER_H_
#define UNDOBUFFER_H_

#include <memory>
#include <string>
#include <vector>

class Editor;

// Abstract action.
class Action {
public:
  // Perform the action on the supplied Editor object.
  virtual void Do(Editor &doc) = 0;
  // Return an action representing the opposite action.
  virtual std::unique_ptr<Action> MakeUndoAction() = 0;
  virtual std::unique_ptr<Action> MakeClone() = 0;
  virtual ~Action() {}

protected:
  static std::string *BufferPointer(Editor &doc);
};

class WriteAction : public Action {
public:
  explicit WriteAction(const std::string &s) : s_(s) {}
  void Do(Editor &doc);
  std::unique_ptr<Action> MakeUndoAction();
  std::unique_ptr<Action> MakeClone();

private:
  std::string s_;
};

class DeleteAction : public Action {
public:
  explicit DeleteAction(const std::string &s) : s_(s) {}
  void Do(Editor &doc);
  std::unique_ptr<Action> MakeUndoAction();
  std::unique_ptr<Action> MakeClone();

private:
  std::string s_;
};

// Simple editor class.
class Editor {
  friend class Action;

public:
  Editor() { ResetUndoPointerToHead(); }
  Editor(const Editor &o);
  void ResetUndoPointerToHead() { undo_ptr_ = actions_.size() - 1; }
  bool CanUndo() { return undo_ptr_ >= 0; }

  // Append `s` to the end of the buffer, add an action and update the
  // undo pointer.
  Editor &Write(const std::string &s);
  // Delete the last `len` characters, add an action and update the undo
  // pointer.
  Editor &Delete(int len);
  // Undo the action being pointed to by the undo_ptr_ and decrement the
  // undo pointer.
  Editor &Undo();

  const std::string &GetState() const;
  std::vector<std::string> GetHistory() const;

private:
  std::string buffer_;
  // Record the series of actions with undo
  // information. Semantically this is a list.
  std::vector<std::unique_ptr<Action>> actions_;
  // Index of the undo pointer referring to a location in `actions_`.
  int undo_ptr_;
};

#endif // UNDOBUFFER_H_
