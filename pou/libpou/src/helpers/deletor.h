#ifndef DELETOR_H
#define DELETOR_H

// This wrapper allow to delete all elements of a container of pointer
// like this:
// for_each(v.begin(), v.end(), DeleteObject());
struct DeleteObject {
  template<typename T>
  void operator()(const T* ptr) {
    delete ptr;
  }
};

#endif
