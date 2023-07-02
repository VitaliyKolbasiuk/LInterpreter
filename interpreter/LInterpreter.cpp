#include "LInterpreter.h"

LInterpreter::LInterpreter() {
	m_builtInFuncMap["defun"] = [](const std::list<SExpression>& exprList) -> SExpression {
		for (const auto& item : exprList) {
			std::cerr << "# ";
			item.print();
			std::cerr << std::endl;
		}
		return exprList.front();
		};
}