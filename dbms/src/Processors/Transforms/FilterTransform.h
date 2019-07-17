#pragma once
#include <Processors/ISimpleTransform.h>
#include <Columns/FilterDescription.h>
#include <Interpreters/ExpressionActions.h>

namespace DB
{

class ExpressionActions;
using ExpressionActionsPtr = std::shared_ptr<ExpressionActions>;

/** Has one input and one output.
  * Simply pull a block from input, transform it, and push it to output.
  * If remove_filter_column is true, remove filter column from block.
  */
class FilterTransform : public ISimpleTransform
{
public:
    FilterTransform(
        const Block & header, ExpressionActionsPtr expression, String filter_column_name, bool remove_filter_column);

    String getName() const override { return "FilterTransform"; }

    Status prepare() override;

protected:
    void transform(Chunk & chunk) override;

private:
    ExpressionActionsPtr expression;
    String filter_column_name;
    bool remove_filter_column;

    ConstantFilterDescription constant_filter_description;
    size_t filter_column_position = 0;

    /// Header after expression, but before removing filter column.
    Block transformed_header;

    ExpressionActions::Cache cache;

    void removeFilterIfNeed(Chunk & chunk);
};

}
