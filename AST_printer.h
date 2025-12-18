#pragma once

#include "AST.h"
#include <string>
#include <iostream>
#include <sstream>

//----------------------------------
// Вспомогательное: оператор -> строка
//----------------------------------
inline std::string op_to_string(Operation op) {
  switch (op) {
    case Operation::Add:       return "+";
    case Operation::Substract: return "-";
    case Operation::Multipy:   return "*";
    case Operation::Divide:    return "/";
    case Operation::Power:     return "^";
    case Operation::Negate:    return "-";
  }
  return "?";
}

//----------------------------------
// Инфиксная форма
//----------------------------------
inline std::string to_infix(const Expr& expr) {
  if (std::holds_alternative<NumberExpr>(expr)) {
    return std::to_string(std::get<NumberExpr>(expr).number);
  }

  if (std::holds_alternative<UnaryExpr>(expr)) {
    const auto& u = std::get<UnaryExpr>(expr);
    return "(-" + to_infix(*u.expr) + ")";
  }

  const auto& b = std::get<BinaryExpr>(expr);
  return "(" +
         to_infix(*b.left) + " " +
         op_to_string(b.op) + " " +
         to_infix(*b.right) +
         ")";
}

//----------------------------------
// Префиксная форма
//----------------------------------
inline std::string to_prefix(const Expr& expr) {
  if (std::holds_alternative<NumberExpr>(expr)) {
    return std::to_string(std::get<NumberExpr>(expr).number);
  }

  if (std::holds_alternative<UnaryExpr>(expr)) {
    const auto& u = std::get<UnaryExpr>(expr);
    return "-" + to_prefix(*u.expr);
  }

  const auto& b = std::get<BinaryExpr>(expr);
  return op_to_string(b.op) + " " +
         to_prefix(*b.left) + " " +
         to_prefix(*b.right);
}

//----------------------------------
// Постфиксная форма
//----------------------------------
inline std::string to_postfix(const Expr& expr) {
  if (std::holds_alternative<NumberExpr>(expr)) {
    return std::to_string(std::get<NumberExpr>(expr).number);
  }

  if (std::holds_alternative<UnaryExpr>(expr)) {
    const auto& u = std::get<UnaryExpr>(expr);
    return to_postfix(*u.expr) + " -";
  }

  const auto& b = std::get<BinaryExpr>(expr);
  return to_postfix(*b.left) + " " +
         to_postfix(*b.right) + " " +
         op_to_string(b.op);
}
inline std::string format_number(double num) {
    if (num == static_cast<int>(num)) {
        return std::to_string(static_cast<int>(num));
    } else {
        std::stringstream ss;
        ss.precision(2);
        ss << std::fixed << num;
        std::string result = ss.str();
        while (result.back() == '0') result.pop_back();
        if (result.back() == '.') result.pop_back();
        return result;
    }
}
//----------------------------------
// Текстовая визуализация дерева
//----------------------------------
inline void print_tree(const Expr& expr) {
    // Конвертируем в текстовое представление
    std::vector<std::string> lines(10, std::string(50, ' '));
    
    // Рекурсивная функция для рисования
    auto draw_node = [&](const Expr& e, int row, int col, int spread, auto&& draw) -> void {
        if (std::holds_alternative<NumberExpr>(e)) {
            const auto& num = std::get<NumberExpr>(e);
            std::string val = format_number(num.number);
            for (size_t i = 0; i < val.length(); ++i) {
                if (col + i < lines[row].length()) {
                    lines[row][col + i] = val[i];
                }
            }
        }
        else if (std::holds_alternative<UnaryExpr>(e)) {
            const auto& un = std::get<UnaryExpr>(e);
            // Рисуем унарный минус
            if (col < lines[row].length()) lines[row][col] = '-';
            // Вертикальная связь
            if (row + 1 < lines.size() && col < lines[row + 1].length()) {
                lines[row + 1][col] = '|';
            }
            // Рисуем выражение
            draw(*un.expr, row + 2, col, spread / 2, draw);
        }
        else if (std::holds_alternative<BinaryExpr>(e)) {
            const auto& bin = std::get<BinaryExpr>(e);
            std::string op = op_to_string(bin.op);
            
            // Рисуем оператор
            if (col < lines[row].length()) lines[row][col] = op[0];
            
            // Рисуем связи к детям
            if (row + 1 < lines.size()) {
                // Левая связь
                if (col - spread/2 >= 0 && col - spread/2 < lines[row + 1].length()) {
                    lines[row + 1][col - spread/2] = '/';
}
                // Правая связь
                if (col + spread/2 < lines[row + 1].length()) {
                    lines[row + 1][col + spread/2] = '\\';
                }
            }
            
            // Рекурсивно рисуем детей
            draw(*bin.left, row + 2, col - spread, spread / 2, draw);
            draw(*bin.right, row + 2, col + spread, spread / 2, draw);
        }
    };
    
    // Начинаем рисовать с корня
    draw_node(expr, 0, 25, 8, draw_node);
    
    // Выводим результат, обрезая пустые строки и пробелы
    for (auto& line : lines) {
        // Убираем пробелы справа
        size_t end = line.find_last_not_of(' ');
        if (end != std::string::npos) {
            std::cout << line.substr(0, end + 1) << "\n";
        }
    }
}
