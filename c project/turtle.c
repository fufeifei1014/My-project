/**
 * CSC A48 - Intro to Computer Science II, Summer 2020
 * 
 * This is the program file where you will implement your solution for
 * assignment 1. Please make sure you read through this file carefully
 * and that you understand what is provided and what you need to complete.
 * 
 * You will need to have read the handout carefully. Parts where you have to 
 * implement functionality are clearly labeled TODO.
 * 
 * Be sure to test your work thoroughly, our testing will be extensive and will
 * check that your solution is *correct*, not only that it provides 
 * functionality.
 * 
 * Developed by Mustafa Quraish for CSCA48
 * (c) Mustafa Quraish 2020
 */
#include "imgUtils.c"

// This lets the driver code override the image size if it needs to. Make sure 
// you don't hard-code these values anywhere!
#ifndef SIZEX
  #define SIZEX 512
  #define SIZEY 512
#endif 



/*---------------------------------------------------------------------------*/

/**
 * This struct contains one node of the linked list, which represents a single 
 * command to the Turtle. It's field should include:
 * 
 *  - cmd   : A char array of size 10 holding the command name
 * 
 *  - val   : An integer that stores a parameter for the command (like forward,
 *            backward and colour). 
 * 
 *  - next  : A pointer to a struct of the same type, this is used for the 
 *            linked list
 * 
 * TODO: Complete this struct definition
 ****/

typedef struct cmdnode {
  char cmd[10];
  int val;
  struct cmdnode *next;

 } CmdNode;


const int CMD_COUNT = 7;
const char *CMDS[] = { "penup", "pendown", "colour", "forward", "backward", "right", "left" };

int check_invalid(char cmd[10]) {
  int i = 0;
  for (i=0; i<CMD_COUNT; i++) {
    if (0 ==strcmp(CMDS[i], cmd)) {
      return 1;   /* 表示有效 */
    }
  }
  if (i >= CMD_COUNT) {
    return 0;    /* 表示无效 */
  }
  return 1;
}

/*---------------------------------------------------------------------------*/

CmdNode *newCmdNode(char cmd[10], int val) {
  /** 
   * This function allocates a new CmdNode struct and initializes it's values 
   * based on the input paramaters given. The next pointer is always 
   * initialized to NULL.
   * 
   * If the 'cmd' parameter is not a correct command, then print
   * "Invalid command.\n" and return NULL.
   * 
   * Note that we will always pass in a value here, even if the 
   * command doesn't need one. In this case, we can just ignore 
   * it. It saves us from having to make separate functions to 
   * deal with this.
   * 
   * TODO: Implement this function
   */
  CmdNode *new_cmd = (CmdNode *)calloc(1, sizeof(CmdNode));
  int valid = check_invalid(cmd);
  if (valid) {
    strcpy(new_cmd -> cmd, cmd);
    new_cmd -> val = val;
    new_cmd -> next = NULL;
  } else {
    printf("Invalid command.\n");
    return NULL;
  }
  return new_cmd; //return 新分配的CmdNode结构
}

/*---------------------------------------------------------------------------*/

void printCommandList(CmdNode *head) {
  /**
   * This function prints out each command in the linked list one after the 
   * other. Each command MUST also have a line number printed before it, this 
   * is what you will be using to modify / delete them. To do this, initialize 
   * a counter and then increment it for each command.
   * 这个函数一个接一个地打印出链表中的每个命令。每个命令之前都必须打印行号，这是为了用来修改/删除它们的行号。为此，初始化一个计数器，然后为每个命令递增计数器。
   * 
   * Depending on whether or not the command needs an additional value 
   * (like forward, backward and colour), use one of the following statements 
   * to print out the information for each node: 
   * 根据命令是否需要额外的值(如forward、backward和colour)，使用以下语句之一打印出每个节点的信息:
   *            [ The format is "linenumber: command value" ]
   * 
   * printf("%3d: %s %d\n", ... );     [With a value]
   * 
   * printf("%3d: %s\n", ... );        [Without a value]
   * 
   * Obviously, you also need to pass in the correct parameters to the print 
   * function yourself, this is just so you have the correct format.(???你要干啥)
   * 
   * TODO: Implement this function
   */
  int counter = 0;   //行号
  CmdNode *p = head;
  
  while (p != NULL) {
    if ((0 == strcmp(p->cmd, "penup")) ||
        (0 == strcmp(p->cmd, "pendown")) ||
        (0 == strcmp(p->cmd, "right")) ||
        (0 == strcmp(p->cmd, "left"))) {
      printf("%3d: %s\n", counter, p->cmd);
    } else {
      printf("%3d: %s %d\n", counter, p->cmd, p->val);
    }
    p = p->next;
    counter++;
  }
  return;
}

/*---------------------------------------------------------------------------*/

void queryByCommand(CmdNode *head, char cmd[10]) {
  /**
   * This function looks for commands in the linked list that match the input 
   * query. It prints them out in the same format as the printCommandList() 
   * function.
   * 
   * Make sure that the line number of the commands that match is the same as 
   * the line number that would be printed by the printCommandList() function.
   * 这个函数在链表中查找与输入查询相匹配的命令。它以与printCommandList()函数相同的格式打印出来。

     确保匹配的命令的行号与printCommandList()函数将打印的行号相同。
   * --------------------------------------------------------------------------
   * 
   * For instance, if your printCommandList function outputs
   * 
   *    0: penup
   *    1: forward 200
   *    2: right
   *    3: forward 50
   * 
   * Then, if this function is called with the same list and cmd = "forward", 
   * then your output here should be
   * 
   *    1: forward 200
   *    3: forward 50
   * 
   * TODO: Implement this function
   */

  int counter = 0;
  CmdNode *p = head; 

  while (p != NULL) {

    if (0 == strcmp(p->cmd, cmd)) {
      if ((0 == strcmp(p->cmd, "penup")) ||
          (0 == strcmp(p->cmd, "pendown")) ||
          (0 == strcmp(p->cmd, "right")) ||
          (0 == strcmp(p->cmd, "left"))) {
        printf("%3d: %s\n", counter, p->cmd);
      } else {
        printf("%3d: %s %d\n", counter, p->cmd, p->val);
      }
    }
    p = p->next;
    counter++;
  }
  return;
}

/*---------------------------------------------------------------------------*/

int countCommands(CmdNode *head) {
  /**
   * This function counts and returns the length of the linked list. It 
   * requires list traversal.
   * 
   * TODO: Implement this function
   */
  int counter = 0;
  CmdNode *p = head;  
  
  while (p != NULL) {
    p = p->next;
    counter++;
  }
  return counter;
}


/*---------------------------------------------------------------------------*/

CmdNode *insertCommand(CmdNode *head, CmdNode *new_CmdNode) {
  /**
   * This function inserts the node new_CmdNode *at the tail* of the linked 
   * list. (You are adding a command at the end).
   * 
   * If head == NULL, then the linked list is still empty.(???)
   * 
   * It returns a pointer to the head of the linked list with the new node 
   * added into it.
   * 
   * TODO: Implement this function
   */

  CmdNode *tail = head;

  if (head == NULL) {
    return new_CmdNode;
  } 
  while (tail->next != NULL) {
    tail = tail->next; 
  }
  tail->next = new_CmdNode;
  new_CmdNode->next = NULL;
  
  return head;
}


/*---------------------------------------------------------------------------*/

CmdNode *insertCommandBefore(CmdNode *head, CmdNode *new_CmdNode, int cmdNum) {
  /**
   * This function inserts a new node *before* a given Node in the linked list. 
   * 
   * 'cmdNum' is an integer that corresponds to the line number of a command 
   * from the printCommandList() function. Your task is to insert new_CmdNode 
   * *before* the corresponding node in the linked list.
   *
   * --------------------------------------------------------------------------
   *  
   * For instance, if your initial list was
   * 
   *    0: penup
   *    1: forward 200
   *    2: right
   *    3: forward 50
   * 
   * And you added "pendown" before cmdNum = 2, then you will have
   * 
   *    0: penup
   *    1: forward 200
   *    2: pendown
   *    3: right
   *    4: forward 50
   * 
   * --------------------------------------------------------------------------
   * 
   * If there is no command with the given cmdNum (cmdNum >= list size), 
   * then print "Invalid Command Number.\n" to the screen and *do not* 
   * insert the new node.
   * 
   * Returns a pointer to the head of the linked list with the new node added 
   * into it.
   * 
   * TODO: Implement this function
   */

  CmdNode *p = head;
  CmdNode *prev = NULL;
  int i = 0;

  while (p != NULL) {
    if (i == cmdNum) {
      if (i == 0) {
        /* insert before 0 */
        new_CmdNode->next = p;
        return new_CmdNode;
      } else {
        prev->next = new_CmdNode;
        new_CmdNode->next = p;
        return head;
      }
    }
    prev = p;
    p = p->next; 
    i++;
  }

  if (cmdNum > i) {
    printf("Invalid Command Number.\n");
    return head;
  } 
  return head;
}

/*---------------------------------------------------------------------------*/

void updateCommand(CmdNode *head, int cmdNum, char cmd[10], int val) {
  /**
   * This function updates a specific node in the linked list based on the 
   * input parameters.
   * 
   * 'cmdNum' is an integer that corresponds to the line number of a command 
   * from the printCommandList() function. Your task is to update the 'cmd' and 
   * 'val' fields of this node.
   * 
   * If there is no command with the given cmdNum, then print 
   * "Invalid Command Number.\n" to the screen, and if 'cmd' is not a correct 
   * command, then print "Invalid command.\n". In both these cases, do *not* 
   * modify the list.
   * 
   * TODO: Implement this function
   */

  int i = 0;
  int valid = 0;
  CmdNode *p = head;
  

  valid = check_invalid(cmd);
  if (!valid) {
    printf("Invalid command.\n");
    return;
  }

  while (p != NULL) {
    if (i == cmdNum) {
      strcpy(p->cmd, cmd);
      p->val = val;
      break;
    }
    p = p->next; 
    i++;
  }

  if (i > cmdNum) {
    printf("Invalid Command Number.\n");
  }
  return;
}

/*---------------------------------------------------------------------------*/


CmdNode *deleteCommand(CmdNode *head, int cmdNum) {
  /**
   * This function deletes the node from the linked list that corresponds to 
   * the line number cmdNum. If there is no command with the given cmdNum, then 
   * the function does nothing.
   * 
   * Returns a pointer to the head of the linked list (which may have changed 
   * as a result of the deletion)
   *
   * TODO: Implement this function
   */
  int i = 0;
  int valid = 0;
  CmdNode *p = head;
  CmdNode *prev = NULL;
  CmdNode *newHead = NULL;

  while (p != NULL) {
    if (i == cmdNum) {
      if (0 == i) {
        /* 删除第一个 */
        newHead = p->next;
        free(p);
        return newHead;
      } else {
        /* 删除其他 */
        prev->next = p->next;
        free(p);
        return head;
      }
    } 
    prev = p;
    p = p->next;
    i++;
  }
  return head;
}

/*---------------------------------------------------------------------------*/


CmdNode *deleteCommandList(CmdNode *head) {
  /**
   * This function deletes the linked list of commands, releasing all the 
   * memory allocated to the nodes in the linked list.
   * 
   * Returns a NULL pointer so that the head of the list can be set to NULL
   * after deletion.
   * 
   * TODO: Implement this function
   */
  CmdNode *p = head;
  CmdNode *next = NULL;

  while (p != NULL) {
    CmdNode *next = p->next;
    free(p);
    p = next;
  }
  return NULL;
}

/*---------------------------------------------------------------------------*/

void run(Image *im, CmdNode *head, int *endX, int *endY) {
  /**
   * This function runs the list of commands to move the turtle around and draw 
   * the image, and returns the final position of the turtle in the variables 
   * endX and endY.
   * 
   * --------------------------------------------------------------------------
   * 
   * NOTE: In the image we work with, the top-left pixel is (0,0),
   *       - x increases as you go right, up till SIZEX-1
   *       - y increases as you go down, up till SIZEY-1
   * 
   *                 (0,0)                 (SIZEX-1, 0)
   *                   x------------------------x
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |          IMAGE         |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   x------------------------x
   *             (0, SIZEY-1)            (SIZEX-1, SIZEY-1)
   * 
   * The image is in grayscale (black and white), so the colours are numbers 
   * from [0-255] where 0 is black, 255 is white, and the values in between 
   * are varying levels of gray.
   * 
   * You need to understand how the (x,y) values are stored so you know how 
   * they should be updated when you move (down means y increases, etc). You do 
   * not need to use the 'im' variable for anything other than passing it into 
   * the drawLine() function described below.
   * 
   * --------------------------------------------------------------------------
   * 
   * Here's the setup - There is a turtle (with a pen) that walks along the 
   * image. When the pen is down (on the paper), it draws a line along the path 
   * that it walks on. (If you want to play around with this, consider looking 
   * at the `turtle` library in python!)
   * 
   * The turtle initially starts at pixel (0, 0) [at the top left], 
   * facing right (in the positive x direction). The pen starts off
   * as `down`, with the default colour being black (0).
   * 
   * You need to go through the linked list and 'run' the commands to keep 
   * track of the turtles position, and draw the appropriate lines. Here is 
   * what each command should do:
   * 
   *  - penup          : Put the pen up (stop drawing)
   *  - pendown        : Put the pen down (start / continue drawing)
   *  - colour <val>   : Draw lines in colour <val> from now on
   *  - forward <val>  : Move the turtle forward <val> steps (pixels) 
   *                     in the direction it is facing.
   *  - backward <val> : Same as above, except move backwards
   *  - right          : Turn the turtle to the right by 90 degrees
   *  - left           : Turn the turtle to the left by 90 degrees
   * 
   * NOTE: Make sure that you do *not* go outside the image. For this, set the 
   * maximum and minimum values of x and y to be 0 and SIZEX-1 / SIZEY-1 
   * respectively.
   * 
   * For instance, if the turtle is at (0,0) facing right, and your command is
   * `forward 100000`, it will only go forward till (SIZEX-1, 0), and end 
   * up at the rightmost pixel in that row.
   * 
   * IMPORTANT: Once you are done with all the commands, make sure you save the 
   * final (x,y) location of the turtle into the variables endX and endY.
   * 
   * --------------------------------------------------------------------------
   * 
   * We have already implemented a drawLine() function (in imgUtils.c) which 
   * you should use to actually draw the lines. It takes in the following:
   * 
   *      - a pointer to an image struct (use 'im')
   *      - (x,y) location of start point
   *      - (x,y) location of end point
   *      - a colour to draw the line in [0-255]
   *          
   * Note that this function only draws horizontal and vertical lines, so 
   * either the x values or the y values of both points must be the same. 
   * Both these points *must* also be within the image. 
   *          [ 0 <= x < SIZEX,  0 <= y < SIZEY ]
   * 
   * 
   * TODO: Implement this function
   */

  int curX = 0, curY = 0;
  int nextX = 0, nextY = 0;
  int curColor = 0;
  int penDown = 1;    /* 0:UP   1:DOWN  */
  int curDir = 1;     /* 0:UP   1:RIGHT   2:DOWN   3:LEFT */

  CmdNode *p = head;
  while (p != NULL) {
    if (0 == strcmp(p->cmd, "penup")) {
      penDown = 0;
    } else if (0 == strcmp(p->cmd, "pendown")) {
      penDown = 1;
    } else if (0 == strcmp(p->cmd, "colour")) {
      curColor = p->val;
    } else if (0 == strcmp(p->cmd, "forward")) {
      switch(curDir) {
        case 0: /* UP */
          nextX = curX;
          nextY = curY - p->val;
          if (nextY < 0) {
            nextY = 0;
          }
          break;
        case 1: /* RIGHT */
          nextY = curY;
          nextX = curX + p->val;
          if (nextX > (im->sx - 1)) {
            nextX = im->sx - 1;
          }
          break;
        case 2: /* DOWN */
          nextX = curX;
          nextY = curY + p->val;
          if (nextY > (im->sy - 1)) {
            nextY = im->sy - 1;
          }
          break;
        case 3: /* LEFT */
          nextY = curY;
          nextX = curX - p->val;
          if (nextX < 0) {
            nextX = 0;
          }
          break;
      }
      if (penDown) {
        drawLine(im, curX, curY, nextX, nextY, curColor);
      }
      curX = nextX;
      curY = nextY;
    } else if (0 == strcmp(p->cmd, "backward")) {
      switch(curDir) {
        case 0: /* UP后退，实际向下 */
          nextX = curX;
          nextY = curY + p->val;
          if (nextY > (im->sy - 1)) {
            nextY = im->sy - 1;
          }
          break;
        case 1: /* RIGHT后退，实际向左 */
          nextY = curY;
          nextX = curX - p->val;
          if (nextX < 0) {
            nextX = 0;
          }
          break;
        case 2: /* DOWN后退，实际向上 */
          nextX = curX;
          nextY = curY - p->val;
          if (nextY < 0) {
            nextY = 0;
          }
          break;
        case 3: /* LEFT后退，实际向右 */
          nextY = curY;
          nextX = curX + p->val;
          if (nextX > (im->sx - 1)) {
            nextX = im->sx - 1;
          }
          break;
      }
      if (penDown) {
        drawLine(im, curX, curY, nextX, nextY, curColor);
      }
      curX = nextX;
      curY = nextY;
    } else if (0 == strcmp(p->cmd, "right")) {
      curDir = (curDir + 1) % 4;
    } else if (0 == strcmp(p->cmd, "left")) {
      curDir = (curDir + 3) % 4;
    }

    p = p->next;
  }
  *endX = curX;
  *endY = curY;
  return;
}

/*---------------------------------------------------------------------------*/
// All done!