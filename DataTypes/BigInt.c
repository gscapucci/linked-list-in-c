#include "BigInt.h"

//--functions-definitions--//
void set_bigint_functions(BigInt *bigint);
void long_long_to_uint8_t(uint8_t *num_array, long long number, size_t lengh);
void str_to_uint8_t(uint8_t *num_array, const char *number, uint8_t start);

void print_bigint(BigInt *self);
void assign_bigint(BigInt *self, BigInt *number);
void assign_int(BigInt *self, long long number);
int compare_bigint_to_bigint(BigInt *self, BigInt *number);
int compare_bigint_to_int(BigInt *self, long long number);
void sum_bigint(BigInt *self, BigInt *number);
void sum_int(BigInt *self, long long number);
void sub_bigint(BigInt *self, BigInt *number);
//------------------------ //

void set_bigint_functions(BigInt *bigint)
{
    bigint->Print = print_bigint;
    bigint->Assign_bigint = assign_bigint;
    bigint->Compare_bigint = compare_bigint_to_bigint;
    bigint->Compare_int = compare_bigint_to_int;
    bigint->Sum_bigint = sum_bigint;
    bigint->Sum_int = sum_int;
    bigint->Sub_bigint = sub_bigint;
}

void long_long_to_uint8_t(uint8_t *num_array, long long number, size_t lengh)
{
    for (size_t i = 0; i < lengh; i++)
    {
        uint8_t resMod10 = number % 10;
        num_array[lengh - i - 1] = resMod10;
        number /= 10;
    }
}

void str_to_uint8_t(uint8_t *num_array, const char *number, uint8_t start)
{
    for (size_t i = 0; i < strlen(number); i++)
    {
        num_array[i] = number[i + start] - '0';
    }
}

void print_bigint(BigInt *self)
{
    if(!self->positive)
    {
        printf("-");
    }
    for (size_t i = 0; i < self->lengh; i++)
    {
        printf("%d", self->_number[i]);
    }
}

void assign_bigint(BigInt *self, BigInt *number)
{
    if(self)
    {
        clear_bigint(self);
        *self = construct_from_bigint(number);
    }
}

void assign_int(BigInt *self, long long number)
{
    if(self)
    {
        clear_bigint(self);
        *self = construct_from_int(number);
    }
}

int compare_bigint_to_bigint(BigInt *self, BigInt *number)
{
    if(self && number)
    {
        if(self->positive != number->positive || self->lengh > number->lengh)
        {
            return self->positive ? 1 : -1;
        }
        if(self->lengh < number->lengh)
        {
            return self->positive ? -1 : 1;
        }
        for (size_t i = 0; i < number->lengh; i++)
        {
            if(self->_number[i] > number->_number[i])
            {
                return 1;
            }
            if(self->_number[i] < number->_number[i])
            {
                return -1;
            }
        }
        return 0;
    }
    fprintf(stderr, "one or two inputs was not valid");
    exit(1);
    return -2;
}

int compare_bigint_to_int(BigInt *self, long long number)
{
    if(self)
    {
        BigInt aux = construct_from_int(number);
        int ret = self->Compare_bigint(self, &aux);
        clear_bigint(&aux);
        return ret;
    }
    fprintf(stderr, "input was not valid");
    exit(1);
    return 0;
}

void sum_bigint(BigInt *self, BigInt *number)
{
    if(self && number)
    {
        if(self->positive != number->positive)
        {
            switch (self->Compare_bigint(self, number))
            {
            case 0:
                free(self->_number);
                self->_number = (uint8_t *)malloc(sizeof(uint8_t));
                self->_number[0] = 0;
                self->positive = true;
                self->lengh = 1;
                return;
            case -1:
                self->positive = number->positive;
                self->Sub_bigint(self, number);
                return;
            case 1:
                self->Sub_bigint(self, number);
                return;
            default:
                return;
            }
        }
        else if(self->lengh > number->lengh)
        {
            size_t diff = self->lengh - number->lengh;
            uint8_t *numberCopy = (uint8_t *)malloc(self->lengh * sizeof(uint8_t));
            for (size_t i = 0; i < diff; i++)
            {
                numberCopy[i] = 0;
            }
            for (size_t i = diff; i < self->lengh; i++)
            {
                numberCopy[i] = number->_number[i - diff];
            }
            free(number->_number);
            number->_number = (uint8_t *)malloc(self->lengh * sizeof(uint8_t));
            number->lengh = self->lengh;
            memcpy(number->_number, numberCopy, self->lengh * sizeof(uint8_t));
            free(numberCopy);
        }
        else if(self->lengh < number->lengh)
        {
            size_t diff = number->lengh - self->lengh;
            uint8_t *self_numberCopy = (uint8_t *)malloc(number->lengh * sizeof(uint8_t));
            for (size_t i = 0; i < diff; i++)
            {
                self_numberCopy[i] = 0;
            }
            for (size_t i = diff; i < number->lengh; i++)
            {
                self_numberCopy[i] = self->_number[i - diff];
            }
            free(self->_number);
            self->_number = (uint8_t *)malloc(number->lengh * sizeof(uint8_t));
            self->lengh = number->lengh;
            memcpy(self->_number, self_numberCopy, number->lengh * sizeof(uint8_t));
            free(self_numberCopy);
        }
        uint8_t *sum = (uint8_t *)malloc(self->lengh * sizeof(uint8_t) + 1);
        for (size_t i = 0; i < self->lengh + 1; i++)
        {
            sum[i] = 0;
        }
        uint8_t carry = 0;
        size_t sum_pos = 0;
        for (size_t i = self->lengh - 1;; i--)
        {
            uint8_t alg = self->_number[i] + number->_number[i] + carry;
            carry = 0;
            while(alg > 9)
            {
                carry = alg / 10;
                alg %= 10;
            }
            sum[sum_pos] = alg;
            sum_pos++;
            if(i == 0)
            {
                break;
            }
        }
        if(carry > 0)
        {
            sum[self->lengh] = carry;
            self->lengh++;
        }
        free(self->_number);
        self->_number = (uint8_t *)malloc(self->lengh * sizeof(uint8_t));
        self->_number = memcpy(self->_number, sum, self->lengh * sizeof(uint8_t));
        
        for (size_t i = 0; i < self->lengh / 2; i++)
        {
            uint8_t temp = self->_number[i];
            self->_number[i] = self->_number[self->lengh - 1 - i];
            self->_number[self->lengh - 1 - i] = temp;
        }
        free(sum);
        return;
    }
    fprintf(stderr, "invalid inputs");
    exit(1);
}

void sum_int(BigInt *self, long long number)
{
    if(self)
    {
        BigInt aux = construct_from_int(number);
        self->Sum_bigint(self, &aux);
        clear_bigint(&aux);
        return;
    }
    fprintf(stderr, "invaid input");
    exit(1);
}

void sub_bigint(BigInt *self, BigInt *number)
{
    if(self && number)
    {
        if(self->positive ^ number->positive)
        {
            self->Sum_bigint(self, number);
            return;
        }
        else
        {
            int comp = self->Compare_bigint(self, number);
            switch (comp)
            {
            case 0:
                free(self->_number);
                self->_number = (uint8_t *)malloc(sizeof(uint8_t));
                self->_number[0] = 0;
                self->lengh = 1;
                self->positive = true;
                return;
            case 1:
                self->positive  = self->positive;
            case -1:
                //TODO: sub
                return;
            default:
                break;
            }
            return;
        }
    }
    fprintf(stderr, "invalid input");
    exit(1);
}

BigInt create_bigint()
{
    BigInt new_bigint;
    new_bigint._number = (uint8_t *)malloc(sizeof(uint8_t));
    new_bigint._number[0] = 0;
    new_bigint.lengh = 1;
    new_bigint.positive = true;
    set_bigint_functions(&new_bigint);
    return new_bigint;
}

BigInt construct_from_int(long long number)
{
    BigInt new_bigint;
    if(number < 0)
    {
        new_bigint.positive = false;
        number *= -1;
    }
    else
    {
        new_bigint.positive = true;
    }
    new_bigint.lengh = floor(log10(number)) + 1;
    new_bigint._number = (uint8_t *)malloc(new_bigint.lengh * sizeof(uint8_t));
    long_long_to_uint8_t(new_bigint._number, number, new_bigint.lengh);
    set_bigint_functions(&new_bigint);
    return new_bigint;
}

BigInt construct_from_bigint(BigInt *bigint)
{
    BigInt new_bigint;
    new_bigint.lengh = bigint->lengh;
    new_bigint.positive = bigint->positive;
    new_bigint._number = (uint8_t *)malloc(bigint->lengh * sizeof(uint8_t));
    memcpy(new_bigint._number, bigint->_number, new_bigint.lengh * sizeof(uint8_t));
    set_bigint_functions(&new_bigint);
    return new_bigint;
}

BigInt construct_from_string(const char *number)
{
    BigInt new_bigint;

    if(number)
    {
        if(!isdigit(number[0]) && number[0] != '-')
        {
            fprintf(stderr, "invalid input");
            exit(1);
        }
        for (size_t i = 1; i < strlen(number); i++)
        {
            if(!isdigit(number[i]))
            {
                fprintf(stderr, "invalid input");
                exit(1);
            }
        }
        new_bigint.positive = true;
        uint8_t count = 0;
        if(number[0] == '-')
        {
            new_bigint.positive = false;
            count++;
        }
        new_bigint.lengh = strlen(number) - count;
        new_bigint._number = (uint8_t *)malloc((strlen(number) - count) * sizeof(uint8_t));
        str_to_uint8_t(new_bigint._number, number, count);
        set_bigint_functions(&new_bigint);
        return new_bigint;
    }
    fprintf(stderr, "invalid number");
    exit(1);
}

void clear_bigint(BigInt *bigint)
{
    if(bigint)
    {
        if(bigint->_number)
        {
            free(bigint->_number);
            bigint->lengh = 0;
            bigint->_number = NULL;
            bigint->positive = true;
        }
    }
}