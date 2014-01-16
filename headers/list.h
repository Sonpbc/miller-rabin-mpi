/* 
 * File:   list.h
 * Author: Zosia
 *
 * Created on January 16, 2014, 9:02 PM
 */

#ifndef LIST_H
#define	LIST_H

#include <cstdio>

struct Entry {
  long val;
};

class List {
public:

  static List* create() {
    return new List();
  }

  static void destroy(List* list) {
    List *curr = list->nxt;
    List *prev;
    while (curr) {
      prev = curr;
      curr = curr->nxt;
      delete prev;
    }
    delete list;
  }

  Entry *current() {
    return &(curr->nxt->entry);
  }

  void append(long value) { // @todo add in the middle
    len++;
    id++;
    curr->nxt = new List();
    curr = curr->nxt;
    curr->entry.val = value;
    curr->nxt = NULL;
  }

  Entry *next() {
    curr = curr->nxt;
    if (curr->nxt) {
      id++;
      return &(curr->nxt->entry);
    } else {
      return NULL;
    }
  }

  void first() {
    curr = this;
    id = 0;
  }

  long length() {
    return len;
  }

  long index() {
    return id;
  }
private:

  long len;

  long id;

  Entry entry;

  List* curr;

  List* nxt;

  List() {
    nxt = NULL;
    curr = this;
    len = 0;
    id = 0;
  }
};


#endif	/* LIST_H */

