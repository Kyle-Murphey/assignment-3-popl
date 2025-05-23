#include "icode.hh"

itab_t * itab_create ()
{
  itab_t * ret;
  ret = new itab_t;
  return ret;
}

int itab_instruction_add (itab_t * itab, int opcode, int addr1, int addr2, int addr3)
{
  icode_t * instr;
  instr = new icode_t;
  instr->op_code = opcode;
  instr->addr1 = addr1;
  instr->addr2 = addr2;
  instr->addr3 = addr3;
  #ifdef _SMP_DEBUG_
    cout << "Showing newly created instruction: ";
    icode_show (instr);  
  #endif
  itab->tab.push_back (instr);
  return itab->tab.size () - 1;
}

void itab_free_table (itab_t * itab)
{
  int ii;
  for (ii = 0; ii < itab->tab.size (); ii++)
    free (itab->tab[ii]);
  itab->tab.clear ();
  free (itab);
}

void icode_show (icode_t * icode)
{
  const char * opstr;
  switch (icode->op_code) 
  {
    case OP_LOAD:
      opstr = "LOAD";
      break;
    case OP_LOADCST:
      opstr = "LOAD_CST";
      break;
    case OP_STORE:
      opstr = "STORE";
      break;
    case OP_ADD:
      opstr = "ADD";
      break;
    case OP_SUB:
      opstr = "SUB";
      break;
    case OP_MUL:
      opstr = "MUL";
      break;
    case OP_DIV:
      opstr = "DIV";
      break;
    case OP_FADD:
      opstr = "FADD";
      break;
    case OP_FSUB:
      opstr = "FSUB";
      break;
    case OP_FMUL:
      opstr = "FMUL";
      break;
    case OP_FDIV:
      opstr = "FDIV";
      break;
    case OP_UMIN:
      opstr = "UMIN";
      break;
    case OP_WRITE:
      opstr = "WRITE";
      break;
    case OP_READ:
      opstr = "READ";
      break;
    default: assert (0 && "Unsupported operation");
  }
  printf ("%s, %d, %d, %d\n", opstr, icode->addr1, icode->addr2, icode->addr3);
}

void itab_show (itab_t * itab)
{
  int ii;
  for (ii = 0; ii < itab->tab.size (); ii++)
  {
    printf ("Instruction %d: ", ii);
    icode_show (itab->tab[ii]);
  }
}

char * prepare_runtime (symtab_t * symtab, itab_t * itab) 
{
  mapsym_t::iterator ss;
  int stack_size = 0;
  for (ss = symtab->map->begin (); ss != symtab->map->end (); ss++)
  {
    if (ss->second->datatype == DTYPE_INT)
      stack_size += sizeof (int);
    if (ss->second->datatype == DTYPE_FLOAT)
      stack_size += sizeof (float);
  }

  char * stack = new char[stack_size];
  memset ((void*)stack, 0, stack_size);
  
  #ifdef _SMP_DEBUG_
  printf ("Allocated and initialized %d bytes\n", stack_size);
  #endif
  return stack;
}

int run (itab_t * itab, char * stack, char * static_mem)
{
  int ii;
  for (ii = 0; ii < itab->tab.size (); ii++)
  {
    icode_t * op = itab->tab[ii];
    switch (op->op_code) 
    {
      case OP_LOAD:
        if (op->addr2 == DTYPE_INT)
        {
          int * src = (int*)(stack + op->addr3);
          int * dst = (int*)(stack + op->addr1);
          *dst = *src;
        }
        // TASK: Complete case for DTYPE_FLOAT
        else if (op->addr2 == DTYPE_FLOAT)
        {
            float * src = (float*)(stack + op->addr3);
            float * dst = (float*)(stack + op->addr1);
            *dst = *src;
        }
        break;
      case OP_LOADCST:
        if (op->addr2 == DTYPE_INT)
        {
          int * src = (int*)(static_mem + op->addr3);
          int * dst = (int*)(stack + op->addr1);
          *dst = *src;
        }
        // TASK: Complete case for DTYPE_FLOAT
        else if (op->addr2 == DTYPE_FLOAT)
        {
            float * src = (float*)(static_mem + op->addr3);
            float * dst = (float*)(stack + op->addr1);
            *dst = *src;
        }
        break;
      case OP_STORE:
        if (op->addr2 == DTYPE_INT)
        {
          int * src = (int*)(stack + op->addr3);
          int * dst = (int*)(stack + op->addr1);
          *dst = *src;
        }
        // TASK: Complete case for DTYPE_FLOAT
        else if (op->addr2 == DTYPE_FLOAT)
        {
            float * src = (float*)(stack + op->addr3);
            float * dst = (float*)(stack + op->addr1);
            *dst = *src;
        }
        break;
      case OP_ADD:
        {
          int * left = (int*)(stack + op->addr2);
          int * right = (int*)(stack + op->addr3);
          int * res = (int*)(stack + op->addr1);
          *res = *left + *right;
        }
        break;
      case OP_SUB:
        {
          int * left = (int*)(stack + op->addr2);
          int * right = (int*)(stack + op->addr3);
          int * res = (int*)(stack + op->addr1);
          *res = *left - *right;
        }
        break;
      case OP_MUL:
        {
          int * left = (int*)(stack + op->addr2);
          int * right = (int*)(stack + op->addr3);
          int * res = (int*)(stack + op->addr1);
          *res = *left * *right;
        }
        break;
      case OP_DIV:
        {
          int * left = (int*)(stack + op->addr2);
          int * right = (int*)(stack + op->addr3);
          int * res = (int*)(stack + op->addr1);
          assert (*right && "Intended division by zero. Aborting");
          *res = *left / *right;
        }
        break;
      case OP_UMIN:
        if (op->addr2 == DTYPE_INT)
        {
          int * src = (int*)(stack + op->addr3);
          int * dst = (int*)(stack + op->addr1);
          *dst = - *src;
        }
        // TASK: Complete case for DTYPE_FLOAT
        else if (op->addr2 == DTYPE_FLOAT)
        {
            float * src = (float*)(stack + op->addr3);
            float * dst = (float*)(stack + op->addr1);
            *dst = - *src;
        }
        break;
      case OP_FADD:
        // TASK: implement this operation. See respective integer operation defined above.
      {
          float * left = (float*)(stack + op->addr2);
          float * right = (float*)(stack + op->addr3);
          float * res = (float*)(stack + op->addr1);
          *res = *left + *right;
      }
        break;
      case OP_FSUB:
        // TASK: implement this operation. See respective floateger operation defined above.
      {
          float * left = (float*)(stack + op->addr2);
          float * right = (float*)(stack + op->addr3);
          float * res = (float*)(stack + op->addr1);
          *res = *left - *right;
      }
        break;
      case OP_FMUL:
        // TASK: implement this operation. See respective floateger operation defined above.
      {
          float * left = (float*)(stack + op->addr2);
          float * right = (float*)(stack + op->addr3);
          float * res = (float*)(stack + op->addr1);
          *res = *left * *right;
      }
        break;
      case OP_FDIV:
        // TASK: implement this operation. See respective floateger operation defined above.
      {
          float * left = (float*)(stack + op->addr2);
          float * right = (float*)(stack + op->addr3);
          float * res = (float*)(stack + op->addr1);
          assert (*right && "Intended division by zero. Aborting");
          *res = *left / *right;
      }
        break;
      case OP_WRITE:
        if (op->addr2 == DTYPE_INT)
        {
          int * addr = (int*)(stack + op->addr1);
          printf ("%d\n", *addr);
        }
        if (op->addr2 == DTYPE_FLOAT)
        {
          float * addr = (float*)(stack + op->addr1);
          printf ("%f\n", *addr);
        }
        break;
      case OP_READ:
        if (op->addr2 == DTYPE_INT)
        {
          int * addr = (int*)(stack + op->addr1);
          scanf ("%d", addr);
        }
        if (op->addr2 == DTYPE_INT)
        {
          float * addr = (float*)(stack + op->addr1);
          scanf ("%f", addr);
        }
        break;
      default: assert (0 && "Unsupported operation");
    }
  }
  return 0;
}
