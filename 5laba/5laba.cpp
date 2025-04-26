#include <iostream>
#include <stack>
#include <queue>
#include <string>
#include <map>
#include <sstream>
#include <cctype>

// Функция определения приоритета операторов
int getPriority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '(') return 0;
    return -1;
}

// Функция преобразования выражения в ОПЗ
std::queue<std::string> convertToRPN(const std::string& expression) {
    std::stack<char> operators;
    std::queue<std::string> output;
    std::string number;

    for (char ch : expression) {
        if (std::isdigit(ch)) {
            number += ch;
        }
        else {
            if (!number.empty()) {
                output.push(number);
                number.clear();
            }
            if (ch == '(') {
                operators.push(ch);
            }
            else if (ch == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    output.push(std::string(1, operators.top()));
                    operators.pop();
                }
                operators.pop(); // Удаление '('
            }
            else if (getPriority(ch) > 0) {
                while (!operators.empty() && getPriority(operators.top()) >= getPriority(ch)) {
                    output.push(std::string(1, operators.top()));
                    operators.pop();
                }
                operators.push(ch);
            }
        }
    }

    if (!number.empty()) {
        output.push(number);
    }

    while (!operators.empty()) {
        output.push(std::string(1, operators.top()));
        operators.pop();
    }

    return output;
}

// Функция вычисления выражения в ОПЗ
double evaluateRPN(std::queue<std::string> rpn) {
    std::stack<double> values;

    while (!rpn.empty()) {
        std::string token = rpn.front();
        rpn.pop();

        if (std::isdigit(token[0])) {
            values.push(std::stod(token));
        }
        else {
            double b = values.top(); values.pop();
            double a = values.top(); values.pop();

            if (token == "+") values.push(a + b);
            else if (token == "-") values.push(a - b);
            else if (token == "*") values.push(a * b);
            else if (token == "/") values.push(a / b);
        }
    }

    return values.top();
}

int main() {
    setlocale(LC_ALL, "ru");
    std::string expression;
    std::map<char, double> variables;

    std::cout << "Введите выражение (например: a-(b/c*(d+e))): ";
    std::getline(std::cin, expression);

    // Запрос значений для переменных
    for (char ch : expression) {
        if (std::isalpha(ch) && variables.find(ch) == variables.end()) {
            std::cout << "Введите значение для " << ch << ": ";
            std::cin >> variables[ch];
        }
    }
    //fhtdhtd
    // Замена переменных их значениями
    std::string processedExpression;
    for (char ch : expression) {
        if (std::isalpha(ch)) {
            processedExpression += std::to_string(variables[ch]);
        }
        else {
            processedExpression += ch;
        }
    }

    // Преобразование в ОПЗ
    std::queue<std::string> rpn = convertToRPN(processedExpression);

    // Вычисление результата
    double result = evaluateRPN(rpn);

    std::cout << "Результат выражения: " << result << std::endl;

    return 0;
}