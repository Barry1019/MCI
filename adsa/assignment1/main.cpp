/*
 * @Author: Shaokang Ma a1790846 Yichun Yuan a1776582
 * @Date: 2020-08-01 20:56:25
 * @LastEditTime: 2020-08-06 15:23:04
 * @Description: • School Method for Integer Addition
                 • Karatsuba Algorithm for Integer Multiplication
 * @FilePath: /MCI/adsa/assignment1/main.cpp
 */ 
#include<iostream>
using namespace std; 

string add(string firstOperand, string secondOperand, string b) {
    int base = std::stoi(b);
    /* make sure the base is no bigger than 10 */
    if (base > 10)
    {
        throw "The base cannot be bigger than 10.";
    }

    /* 
     * add leading zeroes to make 2 strings same long
     * add one more leading zero to form the sum 
     */
    int length1 = firstOperand.length();
    int length2 = secondOperand.length();
    if (length1 > length2)
    {
        for (int i = 0; i < length1 - length2; i++)
        {
            secondOperand = "0" + secondOperand;
        }
    } else if (length2 > length1)
    {
        for (int i = 0; i < length2 - length1; i++)
        {
            firstOperand = "0" + firstOperand;
        }
    }
    firstOperand = "0" + firstOperand;
    secondOperand = "0" + secondOperand;
    
    string sum;
    int carry = 0;
    for (int i = firstOperand.length() - 1; i >= 0; i--)
    {
        int first = firstOperand[i] - '0';
        int second = secondOperand[i] - '0';
        if (first + second + carry >= base)
        {
            sum = to_string(first + second + carry - base) + sum;
            carry = 1;
        } else
        {
            sum = to_string(first + second + carry) + sum;
            carry = 0;
        }
    }
    
    if (sum[0] == '0')
    {
        sum = sum.substr(1, sum.length());
    }
    
    return sum;
}

string primitiveMul(string firstOperand, string secondOperand, string b) {
    /* 
     * a method only to deal with a multiplication when at least one of
     * the operands is one-digit
     */
    int base = std::stoi(b);
    /* make sure the base is no bigger than 10 */
    if (base > 10)
    {
        throw "The base cannot be bigger than 10.";
    }
    string operand;
    int times;

    if (firstOperand.length() == 1)
    {
        times = firstOperand[0] - '0';
        operand = "0" + secondOperand;
    } else
    {
        times = secondOperand[0] - '0';
        operand = "0" + firstOperand;
    }
    
    string product;
    int carry = 0;
    for (int i = operand.length() - 1; i >= 0; i--)
    {   
        int op = operand[i] - '0';
        if (op * times + carry >= base)
        {
            product = to_string((op * times + carry) % base) + product;
            carry = (op * times + carry) / base;
        } else
        {
            product = to_string(op * times + carry) + product;
            carry = 0;
        }
    }
    
    while (product.length() > 1 && product[0] == '0')
    {
        product = product.substr(1, product.length());
    }
    
    return product;
}

string sub(string firstOperand, string secondOperand, string b){
    int base = std::stoi(b);
    /* make sure the base is no bigger than 10 */
    if (base > 10)
    {
        throw "The base cannot be bigger than 10.";
    }
        
    while (secondOperand.length() < firstOperand.length())
    {
        secondOperand = "0" + secondOperand;
    }
    
    int carry = 0;
    string sub;
    for (int i = firstOperand.length() - 1; i >= 0; i--)
    {
        int first = firstOperand[i] - '0';
        int second = secondOperand[i] - '0';
        if (first - second - carry < 0)
        {
            sub = to_string(first + base - second - carry) + sub;
            carry = 1;
        } else
        {
            sub = to_string(first - second - carry) + sub;
            carry = 0;
        }
    }
    while (sub.length() > 1 && sub[0] == '0')
    {
        sub = sub.substr(1, sub.length());
    }
    
    return sub;
}

string multiplyRec(string firstOperand, string secondOperand, string base){
    /* base case */
    if (firstOperand.length() == 1 || secondOperand.length() == 1)
    {
        return primitiveMul(firstOperand, secondOperand, base);
    }
    /* split two operands for 3 situations */
    int length1 = firstOperand.length();
    int length2 = secondOperand.length();
    int n = max(length1, length2);
    int nby2 = n / 2;
    string a, b, c, d;
    if (nby2 >= length1)
    {
        a = "0";
        b = firstOperand;
        c = secondOperand.substr(0, length2 - nby2);
        d = secondOperand.substr(length2 - nby2, nby2);
    }else if (nby2 >= length2)
    {
        a = firstOperand.substr(0, length1 - nby2);
        b = firstOperand.substr(length1 - nby2, nby2);
        c = "0";
        d = secondOperand;
    }else
    {
        a = firstOperand.substr(0, length1 - nby2);
        b = firstOperand.substr(length1 - nby2, nby2);
        c = secondOperand.substr(0, length2 - nby2);
        d = secondOperand.substr(length2 - nby2, nby2);
    }
    
    string ac = multiplyRec(a, c ,base);
    string bd = multiplyRec(b, d, base);
    string adPlusBc = multiplyRec(add(a, b, base), add(c, d, base), base);
    adPlusBc = sub(adPlusBc, ac, base);
    adPlusBc = sub(adPlusBc, bd, base);
    
    for (int i = 0; i < 2 * nby2; i++)
    {
        ac = ac + "0";
    }
    for (int i = 0; i < nby2; i++)
    {
        adPlusBc = adPlusBc + "0";
    }
    /* tailoring the tailng zeroes*/
    while (ac.length() > 1 && ac[0] == '0')
    {
        ac = ac.substr(1, ac.length());
    }
    while (adPlusBc.length() > 1 && adPlusBc[0] == '0')
    {
        adPlusBc = adPlusBc.substr(1, adPlusBc.length());
    }
    
    
    return add(add(ac, adPlusBc, base), bd, base);
}

int main(){
    string input;
    getline(cin,input);
    int end1 = input.find_first_of(" ");
    input[end1] = '*';
    int end2 = input.find(" ");
    string firstOperand = input.substr(0, end1);
    string secondOperand = input.substr(end1 + 1, end2 - end1 - 1);
    string base = input.substr(end2 + 1, input.length() - end2 - 1);    
    
    try
    {
        string sum = add(firstOperand, secondOperand, base);
        string product = multiplyRec(firstOperand, secondOperand, base);
        cout << sum << " " << product << endl;
    }
    catch(const char* msg)
    {
        cerr << msg << endl;
    }
    
    return 0;
}