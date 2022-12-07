#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define COUNT 10

int num_token = 0;

struct node{
    char data[10];
    float value;
    struct node *left;
    struct node *right;
};

struct Stack{
	struct node *data[100];
	int top;
};

void init(struct Stack *stack) {
	stack->top = 0;
}

void push(struct Stack *stack, struct node *n){
	stack->data[stack->top++] = n;
}
void pop(struct Stack *stack){
	stack->top = stack->top -1;
}

int isVar(char *x){
	int i = 0;
	while(x[i]!= '\0'){
		if(isalpha(x[i])) return 1;
		i++; 
	}
	return 0;
}

int isNum(char *x){
	int i = 0;
	while(x[i] != '\0'){
		if(!isdigit(x[i]) && x[i]!='.') return 0;
		i++;
	}
	return 1;
}

struct node *newNode(char c[]){
    struct node *current = (struct node *) malloc(sizeof(struct node));
    for(int i = 0; c[i] != '\0'; i++) current->data[i] = c[i];
    if(isVar(current->data)) current->value = 0.0;
	else if(isNum(current->data)) current->value = atof(current->data);
	else current->value = -1.0;
    current->left = NULL;
	current->right = NULL;
    return current;
};

int isOperator(char c){
	return (c == '+') || (c == '-') || (c == '*') || (c == '/');
}


int isempty(struct Stack *stack) {
	return stack->top == 0;
}

struct node* build(char **tokens){

    struct Stack *stackOperand = (struct Stack *) malloc(sizeof(struct Stack));
    struct Stack *stackOperator = (struct Stack *) malloc(sizeof(struct Stack));
    init(stackOperand);
    init(stackOperator);

 	struct node *t; 
 	struct node *t1; 
 	struct node *t2; 

    int p[256];
    p['+'] = 1;
    p['-'] = 1; 
    p['/'] = 2;
    p['*'] = 2;
    p[')'] = 0;
 
    for (int i = 0; i < num_token; i++){
    	//printf("inside for %s %d %d\n", tokens[i], stackOperator->top, stackOperand->top);
        if (tokens[i][0] == '(') {
        	t = newNode(tokens[i]);
            push(stackOperator, t);
        }
        else if (isalpha(tokens[i][0]) || isdigit(tokens[i][0])){
            t = newNode(tokens[i]);
            push(stackOperand, t);
        }
        else if ((isOperator(tokens[i][0]) || tokens[i][0] == ')') && p[tokens[i][0]] > 0){
            while(!isempty(stackOperator) && (stackOperator->data[(stackOperator->top)-1])->data[0] != '(' && p[(stackOperator->data[(stackOperator->top)-1])->data[0]] >= p[tokens[i][0]]){
                t = newNode((stackOperator->data[(stackOperator->top)-1])->data);
                pop(stackOperator);
             	t1 = stackOperand->data[(stackOperand->top)-1];
                pop(stackOperand);
             	t2 = stackOperand->data[(stackOperand->top)-1];
                pop(stackOperand);
                t->left = t2;
                t->right = t1;
                push(stackOperand, t);
            }
            t = newNode(tokens[i]);
            push(stackOperator, t);
        }
        else if (tokens[i][0] == ')') {
            while (!isempty(stackOperator) && (stackOperator->data[(stackOperator->top)-1])->data[0] != '('){
                t = newNode((stackOperator->data[(stackOperator->top)-1])->data);
              	pop(stackOperator);
             	t1 = stackOperand->data[(stackOperand->top)-1];
                pop(stackOperand);
             	t2 = stackOperand->data[(stackOperand->top)-1];
                pop(stackOperand);
                t->left = t2;
                t->right = t1;
                push(stackOperand, t);
            }
            pop(stackOperator);
        }
    }
    t = stackOperand->data[(stackOperand->top)-1];
    return t;
}

int isOperand(char c){
	return (c == '+') || (c == '-') || (c == '*') || (c == '/');
}

char **tokenize(char *s){
	char **tokens = (char**) calloc(100, sizeof(char*));;
	int i = 0, j = 0, k = 0;
	char temp[100];
	temp[0] = '\0';
	while(s[i]!='\0'){
		if(isOperand(s[i]) || s[i] == '(' || s[i] == ')'){
			if(k!=0){
				tokens[j] = (char*) calloc(k + 1, sizeof(char));
				temp[k] = '\0';
				for(int k_temp = 0; k_temp < k; k_temp++) tokens[j][k_temp] = temp[k_temp];
				// //printf("%s\n", tokens[j]);
				j++;
				num_token += 1;
			}
			tokens[j] = (char*) calloc(2, sizeof(char));
			tokens[j][0] = s[i];
			tokens[j][1] = '\0';
			// //printf("%s\n", tokens[j]);
			j++;
			k = 0;
			num_token+=1;
		}
		else{
			temp[k] = s[i];
			k++;
		}
		i++;
	}
	if(k!=0){
		tokens[j] = (char*) calloc(k + 1, sizeof(char));
		temp[k] = '\0';
		for(int k_temp = 0; k_temp < k; k_temp++) tokens[j][k_temp] = temp[k_temp];
		// //printf("%s\n", tokens[j]);
		j++;
		num_token += 1;
	}
	return tokens;
}

void display_helper(struct node* root, int space){
    if (root == NULL) return;
    space += COUNT;
    display_helper(root->right, space);
    printf("\n");
    for (int i = COUNT; i < space; i++) printf(" ");
    printf("=> %s", root->data);
	if(isVar(root->data)) printf(": %f", root->value);
	printf("\n");
    display_helper(root->left, space);
} 

void display_expression_tree(struct node *tree){
	display_helper(tree, 0);
}

void Preorder(struct node *tree){
	if(tree == NULL) return;
	printf("%s ", tree->data);
	Preorder(tree->left);
	Preorder(tree->right);
}

void Postorder(struct node *tree){
	if(tree == NULL) return;
	Postorder(tree->left);
	Postorder(tree->right);
	printf("%s ", tree->data);
}


int Update(struct node *tree, char *var){
	if(tree == NULL) return 0;
	if(strcmp(tree->data, var) == 0){
		printf("\nEnter the value: \t");
		float val;
		scanf("%f", &val);
		tree->value = val;
		return 1;
	}
	return Update(tree->left, var) || Update(tree->right, var);
}

float operation(float x, float y, char op){
	switch(op){
		case '+':
			return x + y;
		case '-':
			return x - y;
		case '*':
			return x * y;
		case '/':
			if(y == 0){
				printf("Division by zero not supported!\n");
			}
			return x / y;
		default:
			printf("Invalid Operation => only +, -, *, / supported\n");
			return 'x';
	}
}

float Calculate(struct node *tree){
	if(tree == NULL) return 0;
	if(tree->left == NULL && tree->right == NULL) return tree->value;
	return operation(Calculate(tree->left), Calculate(tree->right), tree->data[0]);
}


int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Error: Invalid Expression Passed through Command Line \n\t *(Please do not include any spaces).\n\t *(Expression should contain outer brackets i.e. fully parentheised).\n");
		return 0;
	}
	
	char *expr = argv[1];
	printf("The Entered Expression is %s \n", expr);
	char **tokens = tokenize(expr);
	
	printf("Tokenized = [");
	for(int i = 0; i < num_token; i++){
		printf("%s ", tokens[i]);
	}
	printf("]\n");

	if(tokens[0][0] != '(' && tokens[num_token-1][0] != ')'){
		printf("ERROR: Expression is not fully Parenthesized!\n");
		return 0;
	}
	
	printf("Building Tree ... (if it throws error then Please re-check your expression and re-enter)\n");
	struct node *t = build(tokens);
	printf("Tree Built Successfully! Now you can perform any operations\n");

	int option = 0;
	while(option!=7){

		printf("\n Enter 1 to 7 for the following:\n");
		printf("\t1. Display\n");
		printf("\t2. Preorder\n");
		printf("\t3. Inorder\n");
		printf("\t4. Postorder\n");
		printf("\t5. Update\n");
		printf("\t6. Calculate\n");
		printf("\t7. Exit\n");
		
		scanf("%d", &option);

		printf("==================================================================================================================\n");
		
		switch(option){
			case 1:
				printf("\nDisplaying the tree \n\t * Root in the middle-left, \n\t * left child towards down-right, \n\t * right child towards up-right recursively. \n\n");
				display_expression_tree(t);
				printf("\n NOTE: **(THIS TREE IS ROTATED ANTI-CLOCKWISE 90degrees)**\n\n");
				break;

			case 2:
				printf("\nDisplaying the Preorder Traversal\n");
				Preorder(t);
				printf("\n");
				break;

			case 3:
				printf("\nDisplaying the Inorder Traversal (Fully Parenthesized)\n");
				printf("%s \n", expr);
				break;

			case 4:
				printf("\nDisplaying the Postorder Traversal\n");
				Postorder(t);
				printf("\n");
				break;

			case 5:
				printf("\nEnter the variable name to update: \t");
				char *var_name;
				scanf("%s", var_name);
				if(Update(t, var_name)) printf("Successfully Updated\n");
				else printf("The given variable is not found!\n");
				break;

			case 6:
				printf("\n Calculating the Answer... \t");
				float ans = Calculate(t);
				printf("%f\n", ans);
				break;

			case 7:
				printf("\nExiting the program!\n");
				break;

			default:
				printf("ERROR: Invalid option selected\n");
				break;
		}

		printf("==================================================================================================================\n");
	}


	return 0;
	

}