/*
 * ICGenerator.cc - Implementation of the ICGenerator class
 *
 * Part of the assignment of the Compiler Construction course
 * LIACS, Leiden University
 *
 * This file will contain most of your work for assignment 3.
 */

#include "ICGenerator.h"

bool ICGenerator::is_unary(Node* node) {
  if (node->GetNodeType() == NODE_BOOLEXPR || node->GetNodeType() == NODE_NOT || node->GetNodeType() == NODE_SIGNPLUS ||
      node->GetNodeType() == NODE_SIGNMINUS || node->GetNodeType() == NODE_COERCION) {
    return true;
  }
  return false;
} //is_unary

bool ICGenerator::is_arithmetic(Node* node) {
  NodeType nt = node->GetNodeType();
  if(nt == NODE_ADD || nt == NODE_SUB || nt == NODE_OR || nt == NODE_MUL || nt == NODE_DIV || nt == NODE_IDIV || 
     nt == NODE_MOD || nt == NODE_AND){
     return true;
  }
  return false;
}

void ICGenerator::replace_child(Node* node, Node* root, Node* parent) {
  if (is_unary(parent)) {
    parent->SetChild(node);
  } else {
    if (parent->GetLeftChild() == root) {
      parent->SetLeftChild(node);
    } else if (parent->GetRightChild() == root) {
      parent->SetRightChild(node);
    }
  }
  DeleteNodeRecursive(root);
} //replace_child  

IOperand* ICGenerator::determine_operand(Node* node) {
  IOperand* op = nullptr;
  if (node->GetNodeType() == NODE_NUM_INT) {
    op = new IOperand_Int(node->GetIntValue());
  } else if (node->GetNodeType() == NODE_NUM_REAL) {
    op = new IOperand_Real(node->GetRealValue());
  } else if (node->GetNodeType() == NODE_ID) {
    op = new IOperand_Symbol(node->GetSymbol());
  }
  return op;
} //determine_operand

void ICGenerator::add_tempvar_to_tree(Node* root, Node* parent, Symbol* temp) {
    Node* tempNode = new Node_Symbol;
    tempNode->SetNodeType(NODE_ID);
    tempNode->SetSymbol(temp);
    tempNode->SetReturnType(temp->GetReturnType());
    if (is_unary(parent)) {
      parent->SetChild(tempNode);
    } else {
      if (root == parent->GetLeftChild()) {
        parent->SetLeftChild(tempNode);
      } else if (root == parent->GetRightChild()) {
        parent->SetRightChild(tempNode);
      }
    }
    DeleteNodeRecursive(root);
} //add_tempvar_to_tree

// Constructor
ICGenerator::ICGenerator() {
}

// Destructor
ICGenerator::~ICGenerator() {
}

void ICGenerator::make_node_zero (bool integer, Node* root, Node* parent) {
  Node* newNode;
  
  if (integer) {
    newNode = new Node_Integer;
    newNode->SetNodeType(NODE_NUM_INT);
    newNode->SetReturnType(RT_INT);
    newNode->SetIntValue(0);
    replace_child(newNode, root, parent);
  } else {
    newNode = new Node_Real;
    newNode->SetNodeType(NODE_NUM_REAL);
    newNode->SetReturnType(RT_REAL);
    newNode->SetRealValue(0.0);
    replace_child(newNode, root, parent);
  }
} //make_node_zero

bool ICGenerator::is_zero(Node* child, bool &integer) {
  if (child->GetNodeType() == NODE_NUM_INT) {
    if (child->GetIntValue() == 0) {
      integer = true;
      return true;
    }
  } else if (child->GetNodeType() == NODE_NUM_REAL) {
    if (child->GetRealValue() == 0.0) {
      integer = false;
      return true;
    }
  } else if (child->GetNodeType() == NODE_COERCION) {
    if (child->GetChild()->GetNodeType() == NODE_NUM_INT) {
      if (child->GetChild()->GetIntValue() == 0) {
       integer = false;
        return true;
      }
    }
  }
  return false;
} //is_zero

bool ICGenerator::is_call(Node* node) {
  NodeType nt = node->GetNodeType();
  if (nt == NODE_FUNCTIONCALL || nt == NODE_PROCCALL) {
    return true;
  } else if (nt == NODE_COERCION) {
    if (node->GetChild()->GetNodeType() == NODE_FUNCTIONCALL || node->GetChild()->GetNodeType() == NODE_PROCCALL) {
      return true;
    }
  }
  return false;
} //is_call

bool ICGenerator::check_node_zeroproduct (Node* root, Node* parent) {
  bool integer;
  if (root->GetNodeType() == NODE_MUL) {
    if (is_zero(root->GetLeftChild(), integer) && !is_call(root->GetRightChild())) {
      make_node_zero(integer, root, parent);
      return true;
    } else if (is_zero(root->GetRightChild(), integer) && !is_call(root->GetLeftChild())) {
      make_node_zero(integer, root, parent);
      return true;
    }
  }
  return false;  
} //check_node_zero

bool ICGenerator::make_node_identity(bool replaceleft, Node* root, Node* parent) {
  Node* newNode;

  // replace root with it's right child
  if (!replaceleft) {
    if (root->GetRightChild()->GetNodeType() == NODE_ID) {
      newNode = new Node_Symbol;
      newNode->SetNodeType(NODE_ID);
      newNode->SetSymbol(root->GetRightChild()->GetSymbol());
      newNode->SetReturnType(root->GetRightChild()->GetReturnType());
      replace_child(newNode, root, parent);
      return true;
    }
  // replace root with it's left child
  } else {
    if (root->GetLeftChild()->GetNodeType() == NODE_ID) {
      newNode = new Node_Symbol;
      newNode->SetNodeType(NODE_ID);
      newNode->SetSymbol(root->GetLeftChild()->GetSymbol());
      newNode->SetReturnType(root->GetLeftChild()->GetReturnType());
      replace_child(newNode, root, parent);
      return true;
    }
  }
  return false;
} //make_node_identity

bool ICGenerator::is_one(Node* child) {
  if (child->GetNodeType() == NODE_NUM_INT) {
    if (child->GetIntValue() == 1) {
      return true;
    }
  } else if (child->GetNodeType() == NODE_NUM_REAL) {
    if (child->GetRealValue() == 1.0) {
      return true;
    }
  } else if (child->GetNodeType() == NODE_COERCION) {
    if (child->GetChild()->GetNodeType() == NODE_NUM_INT) {
      if (child->GetChild()->GetIntValue() == 1) {
        return true;
      }
    }
  }
  return false;
} //is_one

bool ICGenerator::check_node_identity(Node* root, Node* parent) {
  bool dummy; // bool in is_zero functioncall not used here
  if (root->GetNodeType() == NODE_MUL || root->GetNodeType() == NODE_IDIV || root->GetNodeType() == NODE_DIV) { 
    if (is_one(root->GetLeftChild())) {
      if (make_node_identity(false, root, parent)) {
        return true;
      }
    } else if (is_one(root->GetRightChild())) {
      if (make_node_identity(true, root, parent)) {
        return true;
      }
    }
  } else if (root->GetNodeType() == NODE_ADD || root->GetNodeType() == NODE_SUB) {
    if (is_zero(root->GetLeftChild(), dummy)) {
      if (make_node_identity(false, root, parent)) {
        return true;
      }
    } else if (is_zero(root->GetRightChild(), dummy)) {
      if (make_node_identity(true, root, parent)) {
        return true;
      }
    }
  }
  return false;
} //check_node_identity

void ICGenerator::make_node_folding(int op1, int op2, Node* root, Node* parent) {
  Node* newNode = new Node_Integer; 
  newNode->SetNodeType(NODE_NUM_INT); 
  newNode->SetReturnType(RT_INT);
  NodeType nt = root->GetNodeType();
  switch (nt) {
    case NODE_ADD: // "+"
      newNode->SetIntValue(op1 + op2);
      break;
    case NODE_SUB: // "-"
      newNode->SetIntValue(op1 - op2);
      break;
    case NODE_OR: // "or"
      newNode->SetIntValue(op1 | op2);
      break;
    case NODE_MUL: // "*"
      newNode->SetIntValue(op1 * op2);
      break;
    case NODE_IDIV: // "div"
      newNode->SetIntValue(op1 / op2);
      break;
    case NODE_MOD: // "mod"
      newNode->SetIntValue(op1 % op2);
      break;
    case NODE_AND: // "and"
      newNode->SetIntValue(op1 & op2);
      break;
    default:
      break;
  }
  replace_child(newNode, root, parent);
} //make_node_folding

void ICGenerator::make_node_folding(float op1, float op2, Node* root, Node* parent) {
  Node* newNode = new Node_Real;  
  newNode->SetNodeType(NODE_NUM_REAL); 
  newNode->SetReturnType(RT_REAL);
  NodeType nt = root->GetNodeType();
  switch (nt) {
    case NODE_ADD: // "+"
      newNode->SetRealValue(op1 + op2);
      break;
    case NODE_SUB: // "-"
      newNode->SetRealValue(op1 - op2);
      break;
    case NODE_MUL: // "*"
      newNode->SetRealValue(op1 * op2);
      break;
    case NODE_DIV: // "/"
      newNode->SetRealValue(op1 / op2);
      break;
    default:
      break;
  }
  replace_child(newNode, root, parent);
} //make_node_folding

bool ICGenerator::check_node_folding(Node* root, Node* parent) {
  if (!is_arithmetic(root)) {
    return false;
  }
  
  Node* left = root->GetLeftChild();
  Node* right = root->GetRightChild();
  if (left == nullptr || right == nullptr) {
    return false;
  }
  NodeType leftType = root->GetLeftChild()->GetNodeType();
  NodeType rightType = root->GetRightChild()->GetNodeType();
  
  if (leftType == NODE_NUM_INT && rightType == NODE_NUM_INT) {
    make_node_folding(left->GetIntValue(), right->GetIntValue(), root, parent);
    return true;
  } else if (leftType == NODE_NUM_REAL && rightType == NODE_NUM_REAL) {
    make_node_folding(left->GetRealValue(), right->GetRealValue(), root, parent);
    return true;
  } else if (leftType == NODE_COERCION && left->GetChild()->GetNodeType() == NODE_NUM_INT && rightType == NODE_NUM_REAL) {
    make_node_folding((float)left->GetChild()->GetIntValue(), right->GetRealValue(), root, parent); 
    return true;
  } else if (leftType == NODE_NUM_REAL && rightType == NODE_COERCION && right->GetChild()->GetNodeType() == NODE_NUM_INT) {
    make_node_folding(left->GetRealValue(), (float)right->GetChild()->GetIntValue(), root, parent);
    return true;
  } else if (leftType == NODE_COERCION && left->GetChild()->GetNodeType() == NODE_NUM_INT &&
             rightType == NODE_COERCION && right->GetChild()->GetNodeType() == NODE_NUM_INT) {
    make_node_folding((float)left->GetChild()->GetIntValue(), (float)right->GetChild()->GetIntValue(), root, parent);
    return true;
  }
  return false;   
} //check_node_folding

bool ICGenerator::fold_signbit(Node* node, Node* parent) {
  NodeType nt = node->GetNodeType();
  int intval;
  float realval;
  Node* newNode;
  if (nt == NODE_SIGNPLUS || nt == NODE_SIGNMINUS) {
    if (node->GetChild()->GetNodeType() == NODE_NUM_INT) {
      intval = node->GetChild()->GetIntValue();
      newNode = new Node_Integer;
      newNode->SetNodeType(NODE_NUM_INT);
      newNode->SetReturnType(RT_INT);
      replace_child(newNode, node, parent);
      if (nt == NODE_SIGNPLUS) {
        newNode->SetIntValue(intval);
      } else if (nt == NODE_SIGNMINUS) {
        newNode->SetIntValue(-intval);
      }
      return true;
    } else if (node->GetChild()->GetNodeType() == NODE_NUM_REAL) {
      realval = node->GetChild()->GetRealValue();
      newNode = new Node_Real;
      newNode->SetNodeType(NODE_NUM_REAL);
      newNode->SetReturnType(RT_REAL);
      replace_child(newNode, node, parent);
      if (nt == NODE_SIGNPLUS) {
        newNode->SetRealValue(realval);
      } else if (nt == NODE_SIGNMINUS) {
        newNode->SetRealValue(-realval);
      }
      return true;
    }
  }
  return false;
} //fold_signbit
  

void ICGenerator::optimize_node(Node* root, Node* parent) {
  if (fold_signbit(root, parent)) {
    return;
  }
  if (check_node_zeroproduct (root, parent)) {
    return;
  }
  if (check_node_identity(root, parent)) {
    return; 
  }
  if (check_node_folding(root, parent)) {
    return;
  }
} //optimize_node


void ICGenerator::preprocessTree(Node* root, Node* parent) {
  if (!is_unary(root)) {
    if (root->GetLeftChild() != nullptr) {
      preprocessTree(root->GetLeftChild(), root);
    }
    if (root->GetRightChild() != nullptr) {
      preprocessTree(root->GetRightChild(), root);
    }
  } else {
    if (root->GetChild() != nullptr) {
      preprocessTree(root->GetChild(), root);
    }
  }
  optimize_node(root, parent);
} //preprocessTree

void ICGenerator::convert_functioncall(Node* root, Node* parent, IntermediateCode* icode, SymbolTable* symtab) {
  IOperand* op1;
  IOperand* res;
  IStatement* statement;
  Node* temp = root->GetRightChild();
  Node* l;
  ReturnType rt;
  Symbol* tempvar;
  //Adding parameters to icode
  while (temp->GetNodeType() != NODE_EMPTY) {
    l = temp->GetLeftChild();
    rt = l->GetReturnType();
    if (rt == RT_INT) {
      icode->AppendStatement(new IStatement(IOP_PARAM_I, determine_operand(l), nullptr, nullptr));
    } else if (rt == RT_REAL) {
      icode->AppendStatement(new IStatement(IOP_PARAM_R, determine_operand(l), nullptr, nullptr));
    }
    temp = temp->GetRightChild();
  }
  if (parent->GetNodeType() != NODE_ASSIGNMENT) {
    op1 = new IOperand_Symbol(root->GetLeftChild()->GetSymbol());
    // temporary variable
    tempvar = GenerateTempVar(root->GetReturnType());
    symtab->AddSymbol(tempvar);
    // Add temporary variable to the tree
    add_tempvar_to_tree(root, parent, tempvar);
    // Add instruction to icode
    res = new IOperand_Symbol(tempvar);
    statement = new IStatement(IOP_FUNCCALL, op1, nullptr, res);
    icode->AppendStatement(statement);
  }
} //convert_functioncall

void ICGenerator::convert_proccall(Node* root, IntermediateCode* icode) {
  IStatement* statement;
  IOperand* op1;
  Node* temp = root->GetRightChild();
  Node* l;
  ReturnType rt;
  //Adding parameters to icode
  while (temp->GetNodeType() != NODE_EMPTY) {
    l = temp->GetLeftChild();
    rt = l->GetReturnType();
    if (rt == RT_INT) {
      icode->AppendStatement(new IStatement(IOP_PARAM_I, determine_operand(l), nullptr, nullptr));
    } else if (rt == RT_REAL) {
      icode->AppendStatement(new IStatement(IOP_PARAM_R, determine_operand(l), nullptr, nullptr));
    }
    temp = temp->GetRightChild();
  }
  //Add call to icode
  op1 = new IOperand_Symbol(root->GetLeftChild()->GetSymbol());
  statement = new IStatement(IOP_PROCCALL, op1, nullptr, nullptr);
  icode->AppendStatement(statement);
} //convert_proccall

void ICGenerator::convert_boolexpr(Node* root, Node* parent, IntermediateCode* icode) {
  bool hasElse;
  Symbol* labelWhile;
  Symbol* labelBegin;
  Symbol* labelElse;
  Symbol* labelEnd;
  IStatement* statement;
  IOperator op;
  IOperand* op1;
  
  //Determine if an else statement is present
  hasElse = parent->GetNodeType() == NODE_IF && parent->GetRightChild()->GetNodeType() == NODE_IF_TARGETS;
  
  //Generate required labels
  if (parent->GetNodeType() == NODE_WHILE) {
    labelWhile = GenerateLabel();
  }
  if (hasElse) {
    labelElse = GenerateLabel();
  }
  labelBegin = GenerateLabel();
  labelEnd = GenerateLabel();
      
  //If needed for while, add label above branch instruction
  if (parent->GetNodeType() == NODE_WHILE) {
    op = IOP_LABEL;
    op1 = new IOperand_Symbol(labelWhile);
    statement = new IStatement(op, op1, nullptr, nullptr);
  }
  
  //Add branch instruction
  convert_relational(root->GetChild(), icode, labelBegin);
  
  //Add jump to either end of if-body or beginning of else body
  op = IOP_GOTO;
  if (hasElse) {
    op1 = new IOperand_Symbol(labelElse);
  } else {
    op1 = new IOperand_Symbol(labelEnd);
  }
  statement = new IStatement(op, op1, nullptr, nullptr);
  icode->AppendStatement(statement);
  
  //Add label to beginning of if-body
  op = IOP_LABEL;
  op1 = new IOperand_Symbol(labelBegin);
  statement = new IStatement(op, op1, nullptr, nullptr);
  icode->AppendStatement(statement);
    
  //Add succeeding statements to respective vectors
  if (parent->GetNodeType() == NODE_WHILE) {
    vecGotoWhile.push_back(labelWhile);
  }
  if (hasElse) {
    vecGotoEnd.push_back(labelEnd);
    vecLabelElse.push_back(labelElse);
  }
  vecLabelEnd.push_back(labelEnd);
} //convert_boolexpr

void ICGenerator::convert_relational(Node* root, IntermediateCode* icode, Symbol* labelBegin) {
  NodeType nt = root->GetNodeType();
  IOperator ot = IOP_UNKNOWN;
  IStatement* statement;
  IOperand* op1;
  IOperand* op2;
  IOperand* res;
  switch (nt) {
    case NODE_REL_EQUAL: // "=="
      if (root->GetLeftChild()->GetReturnType() == RT_INT) {
        ot = IOP_BEQ_I;
      } else if (root->GetLeftChild()->GetReturnType() == RT_REAL) {
        ot = IOP_BEQ_R;
      }
      break;
    case NODE_REL_LT: // "<"
      if (root->GetLeftChild()->GetReturnType() == RT_INT) {
        ot = IOP_BLT_I;
      } else if (root->GetLeftChild()->GetReturnType() == RT_REAL) {
        ot = IOP_BLT_R;
      }
      break;
    case NODE_REL_GT: // ">"
      if (root->GetLeftChild()->GetReturnType() == RT_INT) {
        ot = IOP_BGT_I;
      } else if (root->GetLeftChild()->GetReturnType() == RT_REAL) {
        ot = IOP_BGT_R;
      }
      break;
    case NODE_REL_LTE: // "<="
      if (root->GetLeftChild()->GetReturnType() == RT_INT) {
        ot = IOP_BLE_I;
      } else if (root->GetLeftChild()->GetReturnType() == RT_REAL) {
        ot = IOP_BLE_R;
      }
      break;
    case NODE_REL_GTE: // ">="
      if (root->GetLeftChild()->GetReturnType() == RT_INT) {
        ot = IOP_BGE_I;
      } else if (root->GetLeftChild()->GetReturnType() == RT_REAL) {
        ot = IOP_BGE_R;
      }
      break;
    case NODE_REL_NOTEQUAL: // "<>"
      if (root->GetLeftChild()->GetReturnType() == RT_INT) {
        ot = IOP_BNE_I;
      } else if (root->GetLeftChild()->GetReturnType() == RT_REAL) {
        ot = IOP_BNE_R;
      }
      break;
    default:
      break;
  }
  
  //Determine left and right operands
  op1 = determine_operand(root->GetLeftChild());
  op2 = determine_operand(root->GetRightChild());
  
  //Add branch statement to icode
  res = new IOperand_Symbol(labelBegin);
  statement = new IStatement(ot, op1, op2, res);
  icode->AppendStatement(statement);
} //convert_relational

void ICGenerator::convert_assignment(Node* root, IntermediateCode* icode) {
  IOperator ot;
  IStatement* statement;
  IOperand* op1;
  IOperand* res;
  
  //Function assignments are handled differently from the rest
  if (root->GetRightChild()->GetNodeType() == NODE_FUNCTIONCALL) {
    op1 = new IOperand_Symbol(root->GetRightChild()->GetLeftChild()->GetSymbol());
    res = new IOperand_Symbol(root->GetLeftChild()->GetSymbol());
    statement = new IStatement(IOP_FUNCCALL, op1, nullptr, res);
    icode->AppendStatement(statement);
    return;
  }
  
  if (root->GetRightChild()->GetNodeType() == NODE_NUM_INT) {
    op1 = new IOperand_Int(root->GetRightChild()->GetIntValue());
    ot = IOP_ASSIGN_I;
  } else if (root->GetRightChild()->GetNodeType() == NODE_NUM_REAL) {
    op1 = new IOperand_Real(root->GetRightChild()->GetRealValue());
    ot = IOP_ASSIGN_R;
  } else if (root->GetRightChild()->GetNodeType() == NODE_ID) {
    if (root->GetRightChild()->GetReturnType() == RT_INT) {
      ot = IOP_ASSIGN_I;
    } else if (root->GetRightChild()->GetReturnType() == RT_REAL) {
      ot = IOP_ASSIGN_R;
    }
    op1 = new IOperand_Symbol(root->GetRightChild()->GetSymbol());
  }
  
  res = new IOperand_Symbol(root->GetLeftChild()->GetSymbol());
  statement = new IStatement(ot, op1, nullptr, res); 
  icode->AppendStatement(statement);
} //convert_assignment

void ICGenerator::convert_arithmetic(Node* root, Node* parent, IntermediateCode* icode, SymbolTable* symtab) {
  NodeType nt = root->GetNodeType(); 
  ReturnType rt = root->GetReturnType();
  IOperator op;
  IStatement* statement;
  IOperand* op1;
  IOperand* op2;
  IOperand* res;
  Symbol* tempSym;
  switch (nt) {
    case NODE_ADD: // "+"
        if (rt == RT_INT) {
          op = IOP_ADD_I;
        } else if (rt == RT_REAL) {
          op = IOP_ADD_R;
        }
        break;
      case NODE_SUB: // "-"
        if (rt == RT_INT) {
          op = IOP_SUB_I;
        } else if (rt == RT_REAL) {
          op = IOP_SUB_R;
        }
        break;
      case NODE_OR: // "or"
        op = IOP_OR;
        break;
      case NODE_MUL: // "*"
        if (rt == RT_INT) {
          op = IOP_MUL_I;
        } else if (rt == RT_REAL) {
          op = IOP_MUL_R;
        }
        break;
      case NODE_DIV: // "/"
        op = IOP_DIV_R;
        break;
      case NODE_IDIV: // "div"
        op = IOP_DIV_I; 
        break;
      case NODE_MOD: // "mod"
        op = IOP_MOD;
        break;
      case NODE_AND: // "and"
        op = IOP_AND;
        break;
      default:
        //no
        break;
    }
    
    //Determine left and right operands
    op1 = determine_operand(root->GetLeftChild());
    op2 = determine_operand(root->GetRightChild());
    
    //Create and process temporary variable
    tempSym = GenerateTempVar(rt);
    symtab->AddSymbol(tempSym);
    
    //Add to tree
    add_tempvar_to_tree(root, parent, tempSym);
    
    //Do Icode
    res = new IOperand_Symbol(tempSym);
    statement = new IStatement(op, op1, op2, res);
    icode->AppendStatement(statement);
} //convert_arithmetic  

void ICGenerator::convert_unary(Node* root, Node* parent, IntermediateCode* icode, SymbolTable* symtab) {
  NodeType nt = root->GetNodeType(); 
  ReturnType rt = root->GetReturnType();
  IOperator op;
  IStatement* statement;
  IOperand* op1;
  IOperand* res;
  Symbol* tempSym;
  switch (nt) {
    case NODE_NOT:
      op = IOP_NOT;
      break;
    case NODE_SIGNMINUS:
      if (root->GetReturnType() == RT_INT) {
        op = IOP_UNARY_MINUS_I;
      } else if (root->GetReturnType() == RT_REAL) {
        op = IOP_UNARY_MINUS_R;
      }
      break;
    case NODE_COERCION:
      op = IOP_INT_TO_REAL;
      break;
    default:
      break;
  }
  
  //determine left operand
  op1 = determine_operand(root->GetChild());
  
  //Create and process temporary variable
  tempSym = GenerateTempVar(rt);
  symtab->AddSymbol(tempSym);
  
  //Add to tree
  add_tempvar_to_tree(root, parent, tempSym);
  
  //Do Icode
  res = new IOperand_Symbol(tempSym);
  statement = new IStatement(op, op1, nullptr, res);
  icode->AppendStatement(statement);
} //convert_unary

void ICGenerator::convert_if_targets(Node* root, IntermediateCode* icode) {
  IStatement* statement;
  IOperand* op1;
  
  //Add jump between end of if-body and before else-label
  op1 = new IOperand_Symbol(vecGotoEnd.back());
  statement = new IStatement(IOP_GOTO, op1, nullptr, nullptr);
  vecGotoEnd.pop_back();
  icode->AppendStatement(statement);
  
  //Add label to else-body
  op1 = new IOperand_Symbol(vecLabelElse.back());
  statement = new IStatement(IOP_LABEL, op1, nullptr, nullptr);
  vecLabelElse.pop_back();
  icode->AppendStatement(statement);
} //convert_if_targets
  

void ICGenerator::convert_if(Node* root, IntermediateCode* icode) {
  IStatement* statement;
  IOperand* op1;
  
  //Add label after control sequence
  op1 = new IOperand_Symbol(vecLabelEnd.back());
  statement = new IStatement(IOP_LABEL, op1, nullptr, nullptr);
  vecLabelEnd.pop_back();
  icode->AppendStatement(statement);
} //convert_if

void ICGenerator::convert_while(Node* root, IntermediateCode* icode) {
  IStatement* statement;
  IOperand* op1;
  
  //Add jump to beginning of while loop to icode
  op1 = new IOperand_Symbol(vecGotoWhile.back());
  statement = new IStatement(IOP_GOTO, op1, nullptr, nullptr);
  vecGotoWhile.pop_back();
  icode->AppendStatement(statement);
  
  //Add label after control sequence
  op1 = new IOperand_Symbol(vecLabelEnd.back());
  statement = new IStatement(IOP_LABEL, op1, nullptr, nullptr);
  vecLabelEnd.pop_back();
  icode->AppendStatement(statement);
} //convert_while

void ICGenerator::recurseTree(Node* root, Node* parent, IntermediateCode* icode, SymbolTable* symtab) {
  // check if unary or binary
  NodeType nt = root->GetNodeType();
  if (!is_unary(root)) {
    if (root->GetLeftChild() != nullptr) {
      recurseTree(root->GetLeftChild(), root, icode, symtab);
    }
    if (root->GetNodeType() == NODE_IF_TARGETS) {
      convert_if_targets(root, icode);
    }
    if (root->GetRightChild() != nullptr) {
      recurseTree(root->GetRightChild(), root, icode, symtab);
    }
  } else {
    if (root->GetChild() != nullptr) {
      recurseTree(root->GetChild(), root, icode, symtab);
    }
  }
  switch (nt) {
    case NODE_ASSIGNMENT: // "="
      convert_assignment(root, icode);
      break;
    case NODE_PROCCALL:
      convert_proccall(root, icode);
      break;
    case NODE_FUNCTIONCALL:
      convert_functioncall(root, parent, icode, symtab);
      break;
    case NODE_BOOLEXPR:
      convert_boolexpr(root, parent, icode);
      break;
    case NODE_IF:
      convert_if(root, icode);
      break;
    case NODE_WHILE:
      convert_while(root, icode);
      break;
    case NODE_REL_EQUAL: // "=="
    case NODE_REL_LT: // "<"
    case NODE_REL_GT: // ">"
    case NODE_REL_LTE: // "<="
    case NODE_REL_GTE: // ">="
    case NODE_REL_NOTEQUAL: // "<>"
      //convert_relational(root, symtab); nothing for now
      break;
    case NODE_ADD: // "+"
    case NODE_SUB: // "-"
    case NODE_OR: // "or"
    case NODE_MUL: // "*"
    case NODE_DIV: // "/"
    case NODE_IDIV: // "div"
    case NODE_MOD: // "mod"
    case NODE_AND: // "and"
      convert_arithmetic(root, parent, icode, symtab);
      break;
    case NODE_NOT: // "not"
    case NODE_SIGNPLUS: // "+" sign
    case NODE_SIGNMINUS: // "-" sign
    case NODE_COERCION: // int to real coercion node
      convert_unary(root, parent, icode, symtab);
      break;
    default:
      break;
  }
} //recurseTree

// Preprocesses the syntax tree; this method is called before
// GenerateIntermediateCode() if optimizations are enabled
void ICGenerator::Preprocess(SyntaxTree * tree, SymbolTable * symtab) {
  Node* root = tree->GetProgramBody();
  preprocessTree(root, root);
  for (int i = 0; i < tree->GetSubprogramCount(); i++) {
    root = tree->GetSubprogram(i);
    preprocessTree(root, root);
  }
} //Preprocess

// Takes a SyntaxTree and converts it into an IntermediateCode structure
IntermediateCode * ICGenerator::GenerateIntermediateCode(SyntaxTree * inputTree, SymbolTable * symtab) {
  IntermediateCode * icode;
  IStatement * istmt;
  Node* root;
  Symbol* subsimp;

  icode = new IntermediateCode;
  
  /* Process program body */
  root = inputTree->GetProgramBody();
  
  //Convert the syntax tree to intermediate code
  recurseTree(root, root, icode, symtab);
  
  /* Indicate end of program body */
  istmt = new IStatement (IOP_RETURN);
  icode->AppendStatement(istmt);

  /* Now walk through the list of subprograms, and process them one by one */
  for (int i = 0; i < inputTree->GetSubprogramCount(); i++) {
    root = inputTree->GetSubprogram(i);
    subsimp = symtab->GetSymbol(inputTree->GetSubprogramName(i));
    istmt = new IStatement(IOP_SUBPROG, new IOperand_Symbol(subsimp), nullptr, nullptr);
    icode->AppendStatement(istmt);
    recurseTree(root, root, icode, symtab);
    if (subsimp->GetReturnType() == RT_VOID) {
      istmt = new IStatement(IOP_RETURN);
    } else if (subsimp->GetReturnType() == RT_INT) {
      istmt = new IStatement(IOP_RETURN_I, new IOperand_Symbol(subsimp), nullptr, nullptr);
    } else if (subsimp->GetReturnType() == RT_REAL) {
      istmt = new IStatement(IOP_RETURN_R, new IOperand_Symbol(subsimp), nullptr, nullptr);
    }
    icode->AppendStatement(istmt);
  }
  return icode;
} //GenerateIntermediateCode


// Postprocesses the intermediate code; this method is called after
// GenerateIntermediateCode() if optimizations are enabled
void ICGenerator::Postprocess(IntermediateCode * code, SymbolTable * symtab) {
  //not used
} //Postprocess


// Generate a temporary symbol
Symbol * ICGenerator::GenerateTempVar(ReturnType type) {
  Symbol* tempvar = new Symbol;
  tempvar->SetName("t"+std::to_string(tempCount));
  tempCount++;
  tempvar->SetReturnType(type);
  tempvar->SetSymbolType(ST_TEMPVAR);
  return tempvar;
} //GenerateTempVar


// Generates a Symbol for a label
Symbol * ICGenerator::GenerateLabel() {
  Symbol* label = new Symbol;
  label->SetName("l"+std::to_string(labelCount));
  labelCount++;
  label->SetReturnType(RT_VOID);
  label->SetSymbolType(ST_LABEL);
  return label;
} //GenerateLabel
