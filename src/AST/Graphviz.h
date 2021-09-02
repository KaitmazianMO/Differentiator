#include "Visitor.h"
#include "../VisualStudioWarnings.h"

#include <string>
#include <fstream>

/*
 *
 * @todo RAII_FILE 
 *
 */

class graphwiz_Writer
{		
public:
	graphwiz_Writer (const std::string& gv_file);
   ~graphwiz_Writer ();

	enum class Shape
	{
		circle,
		reclangle
	};

	void addVertex (size_t handle, const std::string& label, Shape shape);
	void addEdje (size_t from, size_t to, const std::string& label);

private:
	const std::string  gv_file_name;
	FILE              *file_ptr = nullptr;
};

class GraphvizPrint : public VisitorVoid
{
public:
	GraphvizPrint (const std::string& gv_file_name);

	void visit (NumberNode *)   override;
	void visit (VariableNode *) override;
	void visit (BinOpNode *)    override;
	void visit (FunctionNode *) override;

private:
	graphwiz_Writer gv_writer;	
};