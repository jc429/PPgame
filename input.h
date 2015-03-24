#ifndef _INPUT_
#define _INPUT_

struct InputNode {
  Uint8 input;
  InputNode *prev;
};

void DeleteInputNode(InputNode *node,int steps);


#endif