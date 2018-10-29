#include "undobuffer.h"

#include <algorithm>
#include <cassert>
#include <memory>

std::string *Action::BufferPointer(Editor &doc) { return &doc.buffer_; }

void WriteAction::Do(Editor &doc) { *BufferPointer(doc) += s_; }

std::unique_ptr<Action> WriteAction::MakeUndoAction() {
  return std::make_unique<DeleteAction>(s_);
}

std::unique_ptr<Action> WriteAction::MakeClone() {
  std::unique_ptr<Action> clone;
  clone.reset(new WriteAction(*this));
  return clone;
}

void DeleteAction::Do(Editor &doc) {
  std::string *buffer = BufferPointer(doc);
  assert(s_.size() <= buffer->size());
  buffer->resize(buffer->size() - s_.size());
}

std::unique_ptr<Action> DeleteAction::MakeUndoAction() {
  return std::make_unique<WriteAction>(s_);
}

std::unique_ptr<Action> DeleteAction::MakeClone() {
  std::unique_ptr<Action> clone;
  clone.reset(new DeleteAction(*this));
  return clone;
}

Editor &Editor::Write(const std::string &s) {
  std::unique_ptr<Action> write_action = std::make_unique<WriteAction>(s);
  write_action->Do(*this);
  actions_.push_back(std::move(write_action));
  ResetUndoPointerToHead();
  return *this;
}

Editor &Editor::Delete(int len) {
  std::unique_ptr<Action> delete_action =
      std::make_unique<DeleteAction>(buffer_.substr(buffer_.size() - len));
  delete_action->Do(*this);
  actions_.push_back(std::move(delete_action));
  ResetUndoPointerToHead();
  return *this;
}

Editor &Editor::Undo() {
  assert(CanUndo());
  std::unique_ptr<Action> undo_action = actions_[undo_ptr_]->MakeUndoAction();
  undo_action->Do(*this);
  actions_.push_back(std::move(undo_action));
  undo_ptr_--;
  return *this;
}

const std::string &Editor::GetState() const { return buffer_; }

Editor::Editor(const Editor &o) {
  buffer_ = o.buffer_;
  undo_ptr_ = o.undo_ptr_;
  for (int i = 0; i < o.actions_.size(); i++) {
    actions_.push_back(o.actions_[i]->MakeClone());
  }
}

std::vector<std::string> Editor::GetHistory() const {
  std::vector<std::string> history;
  Editor tmp = *this;
  while (tmp.CanUndo()) {
    history.push_back(tmp.GetState());
    tmp.Undo();
  }
  history.push_back(std::string()); // The initial empty state.
  std::reverse(history.begin(), history.end());
  return history;
}
