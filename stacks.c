#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

struct value {
	int val;
	int ret;
	char type;
	char * str;
	struct value * next;
};


struct stack {
	struct value * top;
	struct stack * left;
	struct stack * right;
};

struct stack * loops;
struct stack * funcs;


char *readstr() {
   int cursize = 0;
   char * str = (char *)malloc(sizeof(char) * 4);
   int maxsize = 4;
   char x;

   while((x = getchar()) != '\n' && x != EOF) {

      if (cursize >= maxsize) {
         str = (char *)realloc(str, sizeof(char) * maxsize * 2);
         maxsize = maxsize * 2;
          }
      *(str + cursize) = x;
      cursize++; }
   return str; }

void val_push(int val, struct stack * s) {
	struct value * new = malloc(sizeof(struct value));
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


struct stack *create_stack() {
	struct stack *new = malloc(sizeof(struct stack));
	new->top = NULL;
	new->left = NULL;
	new->right = NULL;
	return new;
}


void destroy_stack(struct stack * s) {
	struct value * current = s->top;
	struct value * next;

	while(current != NULL) {
		next = current->next;
		free(current);
		current = next; }
	free(s); }

void loop_push(int val, int ret, char type, struct stack * s) {
	struct value *new = malloc(sizeof(struct value));
	new->val = val;
	new->ret = ret;
	new->type = type;
	new->next = s->top;
	s->top = new; }

void func_push(char name, char * str){
	struct value * new = malloc(sizeof(struct value));
	new->str = str;
	new->val = name;
	new->next = funcs->top;
	funcs->top = new;
}


char * get_func(char name, struct stack * s) {
	struct value * current = s->top;

	while( current ) {
		if (current->val == name) return current->str;
		else current = current->next;
	}

	return NULL;

}

struct stack * process(char * str, struct stack * s, int cur){
	int len = strlen(str);
	char c;
	char op = ' ';
	int i;
	for(i = 0; i < len; i++) {
		c = *(str + i);

		if (c == 'v') {
			val_push(cur,s);
			cur = 0;
		}

		else if (c == '`') {
			val_push(cur,s);
		}


		else if (c == '~') {
			int num = s->top->val;
			if (op == ' ') {
				cur = num;
			}

			else {
				if (op == '+') cur = cur + num;
				else if (op == '-') cur = cur - num;
				else if (op == '*') cur = cur * num;
				else if (op == '/') cur = cur / num;
				op = ' ';
			}
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

		else if( c == '#'){
			printf("%d\n", cur);
			}
		else if ( c == '@'){
			printf("%c\n", cur);
			}

		else if( c == '+' || c == '-' || c =='*' || c == '/') op = c;

		else if( c == '[') {
			loop_push(0, i, cur, loops);
		}

		else if ( c == ']') {

			if( loops->top->type != '[') {
				printf("Error: attempted to close a '[' before closing the current '%c'.\n", loops->top->type);
				exit(0);
			}
			else if (s->top == NULL) {
				val_pop(loops);
			}

			else{
				i = loops->top->ret;
			}
		}

		else if (c == '{') {

			int cursize = 0;
   			char * newstr = malloc(sizeof(char) * 4);
   			int maxsize = 4;
   			struct stack * newstack = create_stack();
   			newstack->top = s->top;

  		 	while(*(str + (++i)) != '}') {

      			if (cursize >= maxsize) {
         			newstr = realloc(newstr, sizeof(char) * maxsize * 2);
        			maxsize = maxsize * 2;
         			}
      			*(newstr + cursize) = *(str + i);
      			cursize++;

      		}

      		s->top = (process(newstr, newstack, cur))->top;
      		}

		else if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
      			char * func = get_func(c,funcs);

      			if ( *(str + i + 1) != '=' && (func == NULL)){
      				printf("Error, expected '=' after %c. \n", c);
      				exit(1);
      			}

      			else if ( *(str + i + 1) == '=' && func) {
      				printf("Error, cannot redefine function %c. \n", c);
      				exit(1);
      			}

      			else if (func) {
      				process(func, s, cur);
      			}

      			else {

      				int cursize = 0;
   				char * newstr = malloc(sizeof(char) * 4);
   				int maxsize = 4;
   				struct stack * newstack = create_stack();
   				newstack->top = s->top;
   				char y = '{';
   				y = c;
  				i++;
  				while(*(str + (++i)) != ';') {

      					if (cursize >= maxsize) {
        	 				newstr = realloc(newstr, sizeof(char) * maxsize * 2);
        					maxsize = maxsize * 2;
         					}
      					*(newstr + cursize) = *(str + i);
      					cursize++;

      					}
      			func_push(c, newstr);
			}
		}
	}

	return s;

}






int main() {
	char * str = readstr();

	struct stack * s = create_stack();
	loops = create_stack();
	funcs = create_stack();
	process(str, s, 0);
}
