#include <stdio.h>
#include <stdlib.h>
struct melon_stuck_struct
{
    int capacity;
    int top_loc;
    int *array;
};

typedef struct melon_stuck_struct *melon_stack;

#define initial_top_loc -1

melon_stack new_melon_stack(int capacity)
{
    int *stack_array_ptr = malloc(sizeof(int) * capacity);

    melon_stack stack = malloc(sizeof(struct melon_stuck_struct));

    stack->capacity = capacity;
    stack->top_loc = initial_top_loc;
    stack->array = stack_array_ptr;

    return stack;
}

void pop(melon_stack stack_instance)
{
    stack_instance->top_loc--;
}

void push(melon_stack stack_instance, int element)
{
    stack_instance->top_loc++;
    stack_instance->array[stack_instance->top_loc] = element;
}

int top(melon_stack stack_instance)
{
    return stack_instance->array[stack_instance->top_loc];
}

int is_empty(melon_stack stack_instance)
{
    return stack_instance->top_loc == initial_top_loc;
}

int top_and_pop(melon_stack stack_instance)
{
    int result = top(stack_instance);
    pop(stack_instance);
    return result;
}

void clear(melon_stack stack_instance)
{
    stack_instance->top_loc = -1;
}

void dispose(melon_stack stack_instance)
{
    if (stack_instance != NULL)
    {
        if (stack_instance->array != NULL)
        {
            free(stack_instance->array);
        }
        free(stack_instance);
    }
}

struct result
{
    int status;
    int result;
};

struct result safe_pop(melon_stack stack_instance)
{
    if (stack_instance->top_loc != -1)
    {
        stack_instance->top_loc--;
        struct result result =
            {
                0, 0};
        return result;
    }
    else
    {
        struct result result =
            {
                1, 0};
        return result;
    }
}

struct result safe_top(melon_stack stack_instance)
{
    if (stack_instance->top_loc != -1)
    {
        struct result result =
            {
                0, stack_instance->array[stack_instance->top_loc]};
        return result;
    }
    else
    {
        struct result result =
            {
                1, 0};
        return result;
    }
}

struct result safe_top_and_pop(melon_stack stack_instance)
{
    if (stack_instance->top_loc != -1)
    {
        struct result result =
            {
                0, stack_instance->array[stack_instance->top_loc]};
        pop(stack_instance);
        return result;
    }
    else
    {
        struct result result =
            {
                1, 0};
        return result;
    }
}

#define INPUT_FIELD_CHARS 128
#define POSTFIX_PRASER_STACK_SIZE 256
#define NUMBER_CALCULUS_STACK_SIZE 256
#define ERROR_MESSAGE "unexpected character reached. please check your input and try again.\n"

#define bool int
#define true 1
#define false 0

struct operate_result
{
    bool succeed;
    int answer;
};

typedef struct operate_result operated;

bool read_and_validate(melon_stack postfix_praser_stack, melon_stack number_calculus_stack, int *number_buffer_ptr, bool *number_buffer_is_empty, char input);
operated operate(melon_stack number_calculus_stack, char operator);
int pow(int base, int amp);
int priority_of(char operator);
void push_number_buffer(int *number_buffer, bool *flag_ptr, int input);
int get_and_clear_number_buffer(int *number_buffer, bool *flag_ptr);

int main(void)
{
    char input_field[INPUT_FIELD_CHARS];
    int read_ptr = 0;
    int number_buffer = 0;
    bool number_buffer_is_empty = true;
    melon_stack postfix_praser_stack = new_melon_stack(POSTFIX_PRASER_STACK_SIZE);
    melon_stack number_calculus_stack = new_melon_stack(NUMBER_CALCULUS_STACK_SIZE);

    // INITIALIZE
    for (; read_ptr < INPUT_FIELD_CHARS; read_ptr++)
    {
        input_field[read_ptr] = '\0';
    }

    printf("Hi there, you can enter an algebraic expression and I will calculate it for you. + - * / and ^(exponentiation) are supported. \n");
start:
    while (1)
    {
        clear(postfix_praser_stack);
        clear(number_calculus_stack);
        printf("> ");
        scanf("%s", &input_field);
        read_ptr = 0;
        while (input_field[read_ptr] != '\0')
        {
            if (!read_and_validate(postfix_praser_stack, number_calculus_stack, &number_buffer, &number_buffer_is_empty, input_field[read_ptr]))
            {
                printf(ERROR_MESSAGE);
                goto start;
            }
            read_ptr++;
        }
        push(number_calculus_stack, number_buffer);
        number_buffer = 0; // clear number buffer
        while (!is_empty(postfix_praser_stack))
        {
            struct result r = safe_top_and_pop(postfix_praser_stack);
            if (r.status != 0)
            {
                printf(ERROR_MESSAGE);
                goto start;
            }
            operated o = operate(number_calculus_stack, r.result);
            if (o.succeed != true)
            {
                printf(ERROR_MESSAGE);
                goto start;
            }
        }
        struct result r = safe_top_and_pop(number_calculus_stack);
        if (r.status == 0 && (postfix_praser_stack) && is_empty(number_calculus_stack))
        {
            printf("answer: %d\n", r.result);
        }
        else
        {
            printf(ERROR_MESSAGE);
        }
    };
}

bool read_and_validate(melon_stack postfix_praser_stack, melon_stack number_calculus_stack, int *number_buffer_ptr, bool *number_buffer_is_empty, char input)
{
    if (input >= '0' && input <= '9')
    {
        push_number_buffer(number_buffer_ptr, number_buffer_is_empty, input - 48);
        return true;
    }
    else if (input == '+' || input == '-' || input == '*' || input == '/' || input == '^')
    {
        // operators
        if (*number_buffer_is_empty == true)
        {
            push(number_calculus_stack, get_and_clear_number_buffer(number_buffer_ptr,number_buffer_is_empty));
        }

        while (true)
        {
            struct result top_operator_result = safe_top(postfix_praser_stack);
            if (top_operator_result.status == 1 /*unsucceed*/)
            {
                push(postfix_praser_stack, input);
                break;
            }
            int priority_exist = priority_of(top_operator_result.result);
            int priority_new = priority_of(input);

            if (priority_new < priority_exist)
            {
                operated o = operate(number_calculus_stack, top_operator_result.result);
                if (!o.succeed)
                {
                    return false;
                }
                pop(postfix_praser_stack);
            }
            else if (priority_new == priority_exist)
            {
                operated o = operate(number_calculus_stack, top_operator_result.result);
                if (!o.succeed)
                {
                    return false;
                }
                else
                {
                    return false;
                }
                pop(postfix_praser_stack);
                push(postfix_praser_stack, input);
                break;
            }
            else /*(priority_new > priority_exist)*/
            {
                push(postfix_praser_stack, input);
                break;
            }
        }

        return true;
    }
    else if (input == '(' || input == ')')
    {
        if (input == '(')
        {
            push(postfix_praser_stack, input);
        }
        else
        {
            if (*number_buffer_is_empty == true)
            {
                push(number_calculus_stack, get_and_clear_number_buffer(number_buffer_ptr,number_buffer_is_empty));
            }
            while (true)
            {
                struct result r = safe_top_and_pop(postfix_praser_stack);
                if (r.status != 0)
                {
                    printf(ERROR_MESSAGE);
                    return false;
                }
                if (r.result != '(')
                {
                    operated o = operate(number_calculus_stack, r.result);
                }
                else
                {
                    return true;
                }
            }
        }
    }
    else if (input == ' ' || input == '	')
    {
        // ignore spaces and tabs
        return true;
    }
    else
    {
        return false;
    }
}

int priority_of(char operator)
{
    switch (operator)
    {
    case '+':
        return 1;
    case '-':
        return 1;
    case '*':
        return 128;
    case '/':
        return 128;
    case '^':
        return 256;
    default:
        return 0;
    }
}

operated operate(melon_stack number_calculus_stack, char operator)
{
    struct result right = safe_top_and_pop(number_calculus_stack);
    struct result left = safe_top_and_pop(number_calculus_stack);
    operated o;
    if (right.status != 0 || left.status != 0)
    {
        o.succeed = false;
        o.answer = 0;
        return o;
    }

    o.succeed = true;
    switch (operator)
    {
    case '+':
        o.answer = left.result + right.result;
        break;
    case '-':
        o.answer = left.result - right.result;
        break;
    case '*':
        o.answer = left.result * right.result;
        break;
    case '/':
        o.answer = left.result / right.result;
        break;
    case '^':
        o.answer = pow(left.result, right.result);
        break;
    default:
        o.succeed = false;
    }

    push(number_calculus_stack, o.answer);

    return o;
}

int pow(int base, int amp)
{
    if (amp == 0)
        return 1;
    if (amp == 1)
    {
        return base;
    }
    if (amp % 2 == 0)
    {
        if (amp == 2)
        {
            return base * base;
        }
        else
        {
            return pow(base * base, amp / 2);
        }
    }
    else
    {
        return base * pow(base * base, amp / 2);
    }
}

void push_number_buffer(int *number_buffer, bool *flag_ptr, int input)
{
    *number_buffer = *number_buffer * 10 + input;
    *flag_ptr = true;
}

int get_and_clear_number_buffer(int *number_buffer, bool *flag_ptr)
{
    int i = *number_buffer;
    *number_buffer = 0;
    *flag_ptr = false;
    return i;
}