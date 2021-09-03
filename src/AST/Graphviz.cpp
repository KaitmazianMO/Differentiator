#include "Graphviz.h"

#include "Expression.h"

GraphwizForamter::GraphwizForamter (std::ostream & os) :
	out (os)
{
	out << "digraph G {\n";
}

void GraphwizForamter::addVertex (size_t handle, const std::string &label, Shape shape)
{
	std::string spape_format;							
	if (shape == GraphwizForamter::Shape::reclangle)
		spape_format = "rect";
	else 
		spape_format = "circle";

	out << "\t" << handle << " [shape = \"" << spape_format << "\" " 
		<< "label = \"" << label << "\"]" << std::endl;
	/*fprintf (file_ptr, "\t%zu [shape = \"%s\" label = \"%s\"]\n",
		handle, spape_format.c_str(), label.c_str());*/
}

void GraphwizForamter::addEdje (size_t from, size_t to, const std::string &label)
{	
	out << "\t" << from << " -> " << to << "[label = \"" << label << "\"]" << std::endl;
	//fprintf (file_ptr, "\t%zu -> %zu [label = \"%s\"]\n", from, to, label.c_str());
}

GraphwizForamter::~GraphwizForamter()
{
	out << "}";
}

GraphvizPrinter::GraphvizPrinter (std::ostream &os) :
	gv_writer (os)
{
}

void GraphvizPrinter::visit (NumberNode *pnode)   
{
	if (pnode)
	{
		gv_writer.addVertex (
			reinterpret_cast<size_t> (pnode),
			std::to_string (pnode->val),
			GraphwizForamter::Shape::reclangle
		);
	}
}

void GraphvizPrinter::visit (VariableNode *pnode) 
{										
	if (pnode)
	{
		gv_writer.addVertex (
			reinterpret_cast<size_t> (pnode),
			pnode->name,
			GraphwizForamter::Shape::reclangle
		);
	}
}

void GraphvizPrinter::visit (BinOpNode *pnode)    
{						
	if (pnode)
	{
		pnode->left ->doAction (this);
		pnode->right->doAction (this);
		
		gv_writer.addEdje (reinterpret_cast<size_t> (pnode), reinterpret_cast<size_t> (pnode->left), "");
		gv_writer.addEdje (reinterpret_cast<size_t> (pnode), reinterpret_cast<size_t> (pnode->right), "");
	
		gv_writer.addVertex (
			reinterpret_cast<size_t> (pnode),
			to_string (pnode->op),
			GraphwizForamter::Shape::circle
		); 
	}		 
}

void GraphvizPrinter::visit (FunctionNode *pnode) 
{	
	if (pnode)
	{ 
		pnode->arg->doAction (this);

		gv_writer.addVertex (
			reinterpret_cast<size_t> (pnode), 
			to_string (pnode->func), 
			GraphwizForamter::Shape::circle);		 
		gv_writer.addEdje (
			reinterpret_cast<size_t> (pnode), 
			reinterpret_cast<size_t> (pnode->arg), 
			""
		);		
	}
}

bool GraphvizPrinter::dump (std::ostream &os, ExpressionNode *AST)
{
	GraphvizPrinter gprinter (os);
	if (AST)
	{
		AST->doAction (&gprinter);
		return true;
	}

	return false;
}

bool GraphvizPrinter::dump (const std::string &file_name, ExpressionNode *AST)
{
	std::ofstream file (file_name + ".dot");
	if (file.bad())
		return false;

	return dump (file, AST);
}