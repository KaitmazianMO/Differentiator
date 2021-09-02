#include "Graphviz.h"

#include "Expression.h"

graphwiz_Writer::graphwiz_Writer (const std::string& gv_file) :
	gv_file_name    (gv_file),
	file_ptr (fopen (gv_file_name.c_str(), "wb"))
{
	if (file_ptr)
	{ 
		fprintf (file_ptr, "digraph G {\n");
	}
}

graphwiz_Writer::~graphwiz_Writer()
{
	if (file_ptr)
	{ 
		fprintf (file_ptr, "}\n");
		fclose (file_ptr);
		file_ptr = nullptr;
	}
}

void graphwiz_Writer::addVertex (size_t handle, const std::string &label, Shape shape)
{
	if (file_ptr)
	{
		std::string spape_format;							
		if (shape == graphwiz_Writer::Shape::reclangle)
			spape_format = "rect";
		else 
			spape_format = "circle";

		fprintf (file_ptr, "\t%zu [shape = \"%s\" label = \"%s\"]\n",
			handle, spape_format.c_str(), label.c_str());
	}
}

void graphwiz_Writer::addEdje (size_t from, size_t to, const std::string &label)
{																					  
	if (file_ptr)
	{
		fprintf (file_ptr, "\t%zu -> %zu [label = \"%s\"]\n", from, to, label.c_str());
	}
}

GraphvizPrint::GraphvizPrint (const std::string &gv_file_name) :
	gv_writer (gv_file_name)
{
}

void GraphvizPrint::visit (NumberNode *pnode)   
{
	if (pnode)
	{
		gv_writer.addVertex (
			reinterpret_cast<size_t> (pnode),
			std::to_string (pnode->val),
			graphwiz_Writer::Shape::reclangle
		);
	}
}

void GraphvizPrint::visit (VariableNode *pnode) 
{										
	if (pnode)
	{
		gv_writer.addVertex (
			reinterpret_cast<size_t> (pnode),
			pnode->name,
			graphwiz_Writer::Shape::reclangle
		);
	}
}

void GraphvizPrint::visit (BinOpNode *pnode)    
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
			graphwiz_Writer::Shape::circle
		); 
	}		 
}

void GraphvizPrint::visit (FunctionNode *pnode) 
{	
	if (pnode)
	{ 
		pnode->arg->doAction (this);

		gv_writer.addVertex (
			reinterpret_cast<size_t> (pnode), 
			to_string (pnode->func), 
			graphwiz_Writer::Shape::circle);		 
		gv_writer.addEdje (
			reinterpret_cast<size_t> (pnode), 
			reinterpret_cast<size_t> (pnode->arg), 
			""
		);		
	}
}
