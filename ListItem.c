/*
htop - ListItem.c
(C) 2004-2011 Hisham H. Muhammad
Released under the GNU GPL, see the COPYING file
in the source distribution for its full text.
*/

#include "ListItem.h"

#include "CRT.h"
#include "String.h"
#include "RichString.h"

#include <string.h>
#include <assert.h>
#include <stdlib.h>

/*{
#include "Object.h"

typedef struct ListItem_ {
   Object super;
   char* value;
   int key;
   bool moving;
} ListItem;

}*/

static void ListItem_delete(Object* cast) {
   ListItem* this = (ListItem*)cast;
   free(this->value);
   free(this);
}

static void ListItem_display(Object* cast, RichString* out) {
   ListItem* const this = (ListItem*)cast;
   assert (this != NULL);
   /*
   int len = strlen(this->value)+1;
   char buffer[len+1];
   snprintf(buffer, len, "%s", this->value);
   */
   if (this->moving) {
      RichString_write(out, CRT_colors[DEFAULT_COLOR], CRT_utf8 ? "↕ " : "+ ");
   } else {
      RichString_prune(out);
   }
   RichString_append(out, CRT_colors[DEFAULT_COLOR], this->value/*buffer*/);
}

ObjectClass ListItem_class = {
   .display = ListItem_display,
   .delete = ListItem_delete,
   .compare = ListItem_compare
};

ListItem* ListItem_new(const char* value, int key) {
   ListItem* this = AllocThis(ListItem);
   this->value = strdup(value);
   this->key = key;
   this->moving = false;
   return this;
}

void ListItem_append(ListItem* this, const char* text) {
   int oldLen = strlen(this->value);
   int textLen = strlen(text);
   int newLen = strlen(this->value) + textLen;
   const char* tmp_value = (const char*) realloc(this->value, newLen + 1);
   assert(tmp_value != NULL);
   this->value = tmp_value;
   memcpy(this->value + oldLen, text, textLen);
   this->value[newLen] = '\0';
}

const char* ListItem_getRef(ListItem* this) {
   return this->value;
}

long ListItem_compare(const void* cast1, const void* cast2) {
   ListItem* obj1 = (ListItem*) cast1;
   ListItem* obj2 = (ListItem*) cast2;
   return strcmp(obj1->value, obj2->value);
}

