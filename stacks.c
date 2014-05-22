#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

struct value {
	int val;
	struct value * next; 
};


struct stack {
	struct value * top;
	struct stack * left;
	struct stack * right;
};

char *readstr() {
   int cursize = 0;
   char * str = malloc(sizeof(char) * 4);
   int maxsize = 4;
   char x;
   
   while((x = getchar()) != '\n' && x != EOF) {
   
      if (cursize >= maxsize) {
         str = realloc(str, sizeof(char) * maxsize * 2);
         maxsize = maxsize * 2;
          }
      *(str + cursize) = x;
      cursize++; }
     
   return str; }

void val_push(int val, struct stack * s) {
	struct value *new = malloc(sizeof(struct value));
	new->val = val;
	
	new->next = s->top;
	s->top = new;						 }
		
	
	
int val_pop(struct stack * s) {
	assert(s->top != NULL);
	int val = s->top->val;
	struct value * cur = s->top->next;
	free(s->top);
	s->top = cur;
	return val;  	          }


int val_peek(struct stack * s) {
	assert(s->top != NULL);
	return s->top->val;
	
}
struct stack *create_stack() {
	struct stack *new = malloc(sizeof(struct stack));
	new->top = NULL;
	new->left = NULL;
	new->right = NULL;
	return new; }
	
	
void destroy_stack(struct stack * s) {
	struct value * current = s->top;
	struct value * next;
	
	while(current != NULL) {
		next = current->next;
		free(current);
		current = next; }
	free(s); }
	
	
int main() {
	char * str = readstr();

	int len = strlen(str);
	int cur = 0;
	char c;
	char op = ' ';
	
	struct stack * first = create_stack();
	struct stack * s = first;
	int i = 0;
	struct stack * loops = create_stack();
	for(i; i < len; i++) {
		c = *(str + i);
		
		if (c == 'v') {
			val_push(cur,s);
			cur = 0;
		}
	 
		else if (c == '^') {
			if (op == ' ') {
				cur = val_pop(s); 
			}
 	
			else {
				if (op == '+') cur = cur + val_pop(s);
				else if (op == '-') cur = cur - val_pop(s);
				else if (op == '*') cur = cur * val_pop(s);
				else if (op == '/') cur = cur / val_pop(s);
				op = ' ';
			}
		}
		
		else if (c == '<') {
			if (s ->left == NULL) {
				struct stack * new = create_stack();
				new->right = s;
				s->left = new;
				s = new; 
			}
			
			else s = s->left;
		}
			
		else if (c == '>') {
			if (s ->right == NULL) {
				struct stack * new = create_stack();
				new->left = s;
				s->right = new;
				s = new; 
			}
			
			else s = s->right;
		}	
	  
		else if (c >= '0' && c <= '9') {
		
			if (op == '+') cur = cur + (c - '0');
			else if (op == '-') cur = cur - (c - '0');
			else if (op == '*') cur = cur * (c - '0');
			else if (op == '/') cur = cur / (c - '0');
			else if (op == ' ') cur = (c - '0');
			
			op = ' ';
		}
		
		else if( c == 'p'){
			printf("%d\n", cur);
			}
			
		else if( c == '+' || c == '-' || c =='*' || c == '/') op = c;
		
		else if( c == '[') {
			val_push(i, loops); 
		}
		
		else if ( c == ']') {
			if (s->top == NULL) {
				val_pop(loops); 
			}
			
			else{
				i = val_peek(loops);
			}
		}
		 		 
	}	 		 
	return 0; }
		 
		 
		 
		 
		 
		 
		 
		 
		 
		 
		 
		 
		 
		 
		 
		 
		 
		 
		 
		 
	
	

