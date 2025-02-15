// Implementation of the list module.
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Each node in a doubly linked list is stored in this structure. The user of the
// module does not have any knowledge of nodes.
struct node { struct node *back; item x; struct node *next; };
typedef struct node node;

// A list is to be represented as a circular list. It has to contain a doubly linked list
// of nodes that hold an item each and one special sentinel node that links to the two ends of the list.
// The none field holds this sentinel node. It has a default item, must always be present for
// a list and always sits before the first item node and after the last item node. For a list
// that holds no nodes the back and next fields of the sentinel node point to the sentinel
// node itself. The current node refers to the currently selected item node in the list. If
// the current node is the none node then no item is selected.
struct list { node *none, *current; };
typedef struct list list;

// ---------- ADD YOUR 14 FUNCTIONS HERE TO SOLVE THE COURSEWORK ----------

// Func 1: newList
list *newList(item element) {
    list *lst = malloc(sizeof(list));
    node *newNode = malloc(sizeof(node));
    newNode->x = element;
    newNode->back = newNode;
    newNode->next = newNode;
    lst->none = newNode;
    lst->current = newNode;
    return lst;
}

// Func 2: freeList
void freeList(list *lst) {
    node *nodePointer = lst->none->next;
    while (nodePointer != lst->none) {
        node *nextNode = nodePointer->next;
        free(nodePointer);
        nodePointer = nextNode;
    }
    free(lst->none);
    free(lst);
}

// Func 3: first
void first(list *lst) {
  lst->current = lst->none->next;
}

// Func 4: last
void last(list *lst) {
  lst->current = lst->none->back;
}

// Func 5: none
bool none(list *lst) {
  return lst->current == lst->none;
}

// Func 6: after
bool after(list *lst) {
  if (none(lst)) return false;
  lst->current = lst->current->next;
  return true;
}

// Func 7: before
bool before(list *lst) {
  if (none(lst)) return false;
  lst->current = lst->current->back;
  return true;
}

// Func 8: get
item get(list *lst) {
  if (none(lst)) return lst->none->x;
  return lst->current->x;
}

// Func 9: set
bool set(list *lst, item element) {
  if (none(lst)) return false;
  lst->current->x = element;
  return true;
}

// Func 10: insertAfter
void insertAfter(list *lst, item element) {
  node *newNode = malloc(sizeof(node));
  newNode->x = element;
  newNode->back = lst->current;
  newNode->next = lst->current->next;
  lst->current->next->back = newNode;
  lst->current->next = newNode;
  lst->current = newNode;
}

// Func 11: insertBefore
void insertBefore(list *lst, item element) {
  node *newNode = malloc(sizeof(node));
  newNode->x = element;
  newNode->back = lst->current->back;
  newNode->next = lst->current;
  lst->current->back->next = newNode;
  lst->current->back = newNode;
  lst->current = newNode;
}

// Func 12: deleteToAfter
bool deleteToAfter(list *lst) {
  if (none(lst)) return false;
  node *nodePointer = lst->current;
  lst->current->back->next = lst->current->next;
  lst->current->next->back = lst->current->back;
  lst->current = lst->current->next;
  free(nodePointer);
  return true;
}

// Func 13: deleteToBefore
bool deleteToBefore(list *lst) {
  if (none(lst)) return false;
  node *nodePointer = lst->current;
  lst->current->back->next = lst->current->next;
  lst->current->next->back = lst->current->back;
  lst->current = lst->current->back;
  free(nodePointer);
  return true;
}

// Func 14: reform
void reform(list *lst) {
  if (none(lst)) return;
  node *nodePointer = lst->current;
  node *temp = nodePointer->next;
  nodePointer->next = lst->none;
  lst->none->back = nodePointer;
  lst->current = lst->none;
  while (temp != lst->none) {
    node *nextNode = temp->next;
    temp->next = lst->current;
    lst->current->back = temp;
    lst->current = temp;
    temp = nextNode;
  }
  lst->current = nodePointer;
  lst->current->back = lst->none;
  lst->none->next = lst->current;
}


// Test the list module, using int as the item type. Strings are used as
// 'pictograms' to describe lists. Single digits represent items and the '|' symbol
// in front of a digit indicates that this is the current item. If the '|' symbol
// is at the end of the string then 'none' of the items is selected. The strings
// "|37", "3|7", "37|" represent a list of two items, with the current position
// at the first item, the last item, and a situation where 'none' of the items
// is selected.
#ifdef test_list

// Build a list from a pictogram, with -1 as the default item.
// Note: You do not need to understand this function to solve the coursework.
list *build(char *s) {
  list *xs = malloc(sizeof(list));
    int n = strlen(s);
    node *nodes[n];
    for (int i = 0; i < n; i++) nodes[i] = malloc(sizeof(node));
    for (int i = 0; i < n; i++) nodes[i]->next = nodes[(i + 1) % n];
    for (int i = 1; i < n + 1; i++) nodes[i % n]->back = nodes[i - 1];
    xs->none = nodes[0];
    xs->none->x = -1;
    node *p = xs->none->next;
    for (int i = 0; i < strlen(s); i++) {
      if (s[i] == '|') xs->current = p;
      else {
        p->x = s[i] - '0';
        p = p->next;
      }
    }
  return xs;
}

// Destroy a list which was created with the build function and which matches a pictogram.
// Note: You do not need to understand this function to solve the coursework.
void destroy(list *xs, char *s) {
  int n = strlen(s);
  node *nodes[n];
  nodes[0] = xs->none;
  for (int i = 1; i < n; i++) nodes[i] = nodes[i-1]->next;
  for (int i = 0; i < n; i++) free(nodes[i]);
  free(xs);
}

// Check that a list matches a pictogram.
// Note: You do not need to understand this function to solve the coursework.
bool match(list *xs, char *s) {
  int n = strlen(s);
  node *nodes[n];
  nodes[0] = xs->none;
  for (int i = 1; i < n; i++) nodes[i] = nodes[i - 1]->next;
  if (nodes[n - 1]->next != xs->none) return false;
    for (int i = 1; i < n; i++) {
      if (nodes[i]->back != nodes[i - 1]) return false;
    }
  node *p = xs->none->next;
  for (int i = 0; i < strlen(s); i++) {
    if (s[i] == '|') {
      if (p != xs->current) return false;
    }
    else {
      if (p->x != s[i] - '0') return false;
      p = p->next;
    }
  }
  return true;
}

// The tests use an enumeration to say which function to call.
enum { First, Last, None, After, Before, Get, Set, InsertAfter, InsertBefore, DeleteToAfter, DeleteToBefore, Reform};
typedef int function;

// A replacement for the library assert function.
void assert(int line, bool b) {
  if (b) return;
  printf("The test on line %d fails.\n", line);
  exit(1);
}

// Call a given function with a possible integer argument, returning a possible
// integer or boolean result (or -1).
// Note: You do not need to understand this function to solve the coursework.
int call(function f, list *xs, int arg) {
  int result = -1;
  switch (f) {
    case None: result = none(xs); break;
    case First: first(xs); break;
    case Last: last(xs); break;
    case After: result = after(xs); break;
    case Before: result = before(xs); break;
    case Get: result = get(xs); break;
    case Set: result = set(xs, arg); break;
    case InsertAfter: insertAfter(xs, arg); break;
    case InsertBefore: insertBefore(xs, arg); break;
    case DeleteToBefore: result = deleteToBefore(xs); break;
    case DeleteToAfter: result = deleteToAfter(xs); break;
    case Reform: reform(xs); break;
    default: assert(__LINE__, false);
  }
  return result;
}

// Check that a given function does the right thing. The 'in' value is passed to
// the function or is -1. The 'out' value is the expected result, or -1.
bool check(function f, int in, char *before, char *after, int out) {
  list *xs = build(before);
  int result = call(f, xs, in);
  //check your list has indeed a circular list structure
  assert(__LINE__, (xs->none == xs->none->next->back));
  assert(__LINE__, (xs->none == xs->none->back->next));
  //check that your function works correctly as the tests demand
  bool ok = (match(xs, after) && (result == out));
  destroy(xs, after);
  return ok;
}

// Test newList, and call freeList. The test for freeList is that the memory
// leak detector in the -fsanitize=address or -fsanitize=leak compiler option
// reports no problems.
void testNewList() {
    list *xs = newList(-1);
    //check circular list structure
    assert(__LINE__, (xs->none == xs->none->next));
    assert(__LINE__, (xs->none == xs->none->back));
    //check that an empty list is produced with a sentinel correctly
    assert(__LINE__, match(xs, "|"));
    freeList(xs);
}

// Test the various 14 functions.
void testFirst() {
    assert(__LINE__, check(First, -1, "|", "|", -1));
    assert(__LINE__, check(First, -1, "|37", "|37", -1));
    assert(__LINE__, check(First, -1, "3|7", "|37", -1));
    assert(__LINE__, check(First, -1, "37|", "|37", -1));
}

void testLast() {
    assert(__LINE__, check(Last, -1, "|", "|", -1));
    assert(__LINE__, check(Last, -1, "|37", "3|7", -1));
    assert(__LINE__, check(Last, -1, "3|7", "3|7", -1));
    assert(__LINE__, check(Last, -1, "37|", "3|7", -1));
}

void testNone() {
    assert(__LINE__, check(None, -1, "|", "|", true));
    assert(__LINE__, check(None, -1, "|37", "|37", false));
    assert(__LINE__, check(None, -1, "3|7", "3|7", false));
    assert(__LINE__, check(None, -1, "37|", "37|", true));
}

void testAfter() {
    assert(__LINE__, check(After, -1, "|", "|", false));
    assert(__LINE__, check(After, -1, "|37", "3|7", true));
    assert(__LINE__, check(After, -1, "3|7", "37|", true));
    assert(__LINE__, check(After, -1, "37|", "37|", false));
}

void testBefore() {
    assert(__LINE__, check(Before, -1, "|", "|", false));
    assert(__LINE__, check(Before, -1, "|37", "37|", true));
    assert(__LINE__, check(Before, -1, "3|7", "|37", true));
    assert(__LINE__, check(Before, -1, "37|", "37|", false));
}

void testGet() {
    assert(__LINE__, check(Get, -1, "|", "|", -1));
    assert(__LINE__, check(Get, -1, "|37", "|37", 3));
    assert(__LINE__, check(Get, -1, "3|7", "3|7", 7));
    assert(__LINE__, check(Get, -1, "37|", "37|", -1));
}

void testSet() {
    assert(__LINE__, check(Set, 5, "|", "|", false));
    assert(__LINE__, check(Set, 5, "|37", "|57", true));
    assert(__LINE__, check(Set, 5, "3|7", "3|5", true));
    assert(__LINE__, check(Set, 5, "37|", "37|", false));
}

void testInsertAfter() {
    assert(__LINE__, check(InsertAfter, 5, "|", "|5", -1));
    assert(__LINE__, check(InsertAfter, 5, "|37", "3|57", -1));
    assert(__LINE__, check(InsertAfter, 5, "3|7", "37|5", -1));
    assert(__LINE__, check(InsertAfter, 5, "37|", "|537", -1));
}

void testInsertBefore() {
    assert(__LINE__, check(InsertBefore, 5, "|", "|5", -1));
    assert(__LINE__, check(InsertBefore, 5, "|37", "|537", -1));
    assert(__LINE__, check(InsertBefore, 5, "3|7", "3|57", -1));
    assert(__LINE__, check(InsertBefore, 5, "37|", "37|5", -1));
}

void testDeleteToAfter() {
    assert(__LINE__, check(DeleteToAfter, -1, "|", "|", false));
    assert(__LINE__, check(DeleteToAfter, -1, "|37", "|7", true));
    assert(__LINE__, check(DeleteToAfter, -1, "3|7", "3|", true));
    assert(__LINE__, check(DeleteToAfter, -1, "37|", "37|", false));
    assert(__LINE__, check(DeleteToAfter, -1, "|5", "|", true));
}

void testDeleteToBefore() {
    assert(__LINE__, check(DeleteToBefore, -1, "|", "|", false));
    assert(__LINE__, check(DeleteToBefore, -1, "|37", "7|", true));
    assert(__LINE__, check(DeleteToBefore, -1, "3|7", "|3", true));
    assert(__LINE__, check(DeleteToBefore, -1, "37|", "37|", false));
    assert(__LINE__, check(DeleteToBefore, -1, "|5", "|", true));
}

void testReform() {
    assert(__LINE__, check(Reform, -1, "|", "|", -1));
    assert(__LINE__, check(Reform, -1, "|37", "7|3", -1));
    assert(__LINE__, check(Reform, -1, "3|7", "3|7", -1));
    assert(__LINE__, check(Reform, -1, "37|", "37|", -1));
}

int main() {
    testNewList();
    testFirst();
    testLast();
    testNone();
    testAfter();
    testBefore();
    testGet();
    testSet();
    testInsertAfter();
    testInsertBefore();
    testDeleteToAfter();
    testDeleteToBefore();
    testReform();
    printf("List module tests run OK.\n");
    return 0;
}
#endif
