#ifndef GRAPHVIZ_H_INCLUDED
#define GRAPHVIZ_H_INCLUDED


#include "Visitor.h"
#include "../VisualStudioWarnings.h"

#include <string>
#include <fstream>

class GraphwizForamter
{		
public:
	GraphwizForamter(std::ostream & os);
   ~GraphwizForamter();

	enum class Shape
	{
		circle,
		reclangle
	};

	void addVertex (size_t handle, const std::string& label, Shape shape);
	void addEdje (size_t from, size_t to, const std::string& label);

private:
	std::ostream &out;
};

class GraphvizPrinter : public VisitorVoid
{
public:
	GraphvizPrinter (std::ostream &os);

	void visit (NumberNode *)   override;
	void visit (VariableNode *) override;
	void visit (BinOpNode *)    override;
	void visit (FunctionNode *) override;

	static bool dump (std::ostream &os, ExpressionNode *AST);
	static bool dump (const std::string &file_name, ExpressionNode *AST);

private:
	GraphwizForamter gv_writer;	
};


#endif