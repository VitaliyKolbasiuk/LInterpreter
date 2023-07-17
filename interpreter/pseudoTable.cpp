#include "LInterpreter.h"

void LInterpreter::addPseudoTableFuncs() {

    m_builtInFuncMap["printRect"] = [](List* expr) -> ISExpr* {
        List* parameterList = (List*) expr->m_car;
        int height = atoi( ((Atom*)(parameterList->m_car))->name() );
        int width = atoi( ((Atom*)(parameterList->m_cdr->m_car))->name() );

        std::string answer = "";
        for (int j = 0; j < height; ++j) {
            for (int i = 0; i <= width; ++i) {
                if (i == 0 && j == 0) {
                    answer += ".";
                }
                else if (j == 0 && i + 1 == width) {
                    answer += ".";
                }
                else if (i == 0 && j + 1 == height) {
                    answer += ".";
                }
                else if (i + 1 == width && j + 1 == height) {
                    answer += ".";
                }
                else if (i == 0 || i + 1 == width) {
                    answer += '|';
                }
                else if (i == width) {
                    answer += '\n';
                }
                else if (j == 0 || j + 1 == height) {
                    answer += '_';
                }
                else answer += ' ';
            }
        }
        std::cout << answer;
        return new List();
    };
    return;
}
