#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 1025

struct Operation {
    int ind1;
    int ind2;
    int opcode;
    char** saved;
    int last;
    int checkpoint;
    int savedcap;
};

struct Stack {
    struct Operation* arr;
    int undoSSize, size, cap;
};

char** editor;
struct Stack* stack = NULL;
int capacity = 2;
int lastLine = 0;
int stackCap = 2;
int lastCheckpoint = 0;
int qUndo = 0;
int qRedo = 0;
int redoSSize = 0;

// Stack functions

struct Stack* createStack(int cap);
void doubleStack(struct Stack* s);
int isFull(struct Stack* s);
void push(struct Stack* s,  int pInd1, int pInd2, int pOpcode, char** pSaved, int pLast, int pCheckpoint, int pSavedcap);

// Editor functions
void editorSetNull(int pInd1, int pInd2);
void editorExpand();

// Text functions
void start();

void change(int pInd1, int pInd2);

void delete(int pInd1, int pInd2);
void cleanDeleteIndexes(int* pInd1, int* pInd2);

void print(int pInd1, int pInd2);

void queueUndo(int ind);
void undo(int ind);

void queueRedo(int ind);
void redo(int ind);

void restoreCheckpoint(int ind);
void doChange(char** edd, int ind);
void fixCheckpoint(int* checkpoint);

int main () {
    stack = createStack(stackCap);
    editor = malloc(sizeof(char*) * capacity);
    editorSetNull(0, capacity);
    delete(0,0);
    start();
    return 0;
}


void start()  {
    char instruction[MAXSIZE];
    fgets(instruction, MAXSIZE, stdin);
    while(strcmp(instruction, "q\n") != 0) {
        int len = strlen(instruction);
        char code = instruction[len - 2];

        if (code == 'c') {
            int diff = qUndo-qRedo;

            if(diff > 0) {
                undo(diff);
            }
            if(diff < 0) {
                diff = - diff;
                redo(diff);
            }
            qRedo = 0;
            qUndo = 0;
            if(redoSSize != 0) {
                redoSSize = 0;
                fixCheckpoint(&lastCheckpoint);
            }
            char *token = strtok(instruction, ",");
            int first = atoi(token);
            token = strtok(NULL, ",");
            int second = atoi(token);
            change(first, second);

        }
        if (code == 'd') {
            int diff = qUndo-qRedo;
            if(diff > 0) {
                undo(diff);
            }
            if(diff < 0) {
                diff = - diff;
                redo(diff);
            }
            if(redoSSize != 0) {
                redoSSize = 0;
                fixCheckpoint(&lastCheckpoint);
            }
            char *token = strtok(instruction, ",");
            int first = atoi(token);
            token = strtok(NULL, ",");
            int second = atoi(token);
            delete(first, second);
            qRedo = 0;
            qUndo = 0;

        }
        if (code == 'p') {
            int diff = qUndo-qRedo;

            if(diff > 0) {
                undo(diff);
            }
            if(diff < 0) {
                diff = - diff;
                redo(diff);
            }
            char *token = strtok(instruction, ",");
            int first = atoi(token);
            token = strtok(NULL, ",");
            int second = atoi(token);
            print(first, second);
            qUndo = 0;
            qRedo = 0;
        }
        if (code == 'u') {
            int index = atoi(instruction);
            queueUndo(index);
        }
        if (code == 'r') {
            int index = atoi(instruction);
            queueRedo(index);
        }
        fgets(instruction, MAXSIZE, stdin);
    }
}

void editorSetNull(int pInd1, int pInd2) {
    for(int i = pInd1; i < pInd2; i++) {
        editor[i] = NULL;
    }
}

void editorExpand() {
    char*** editorPtr = &editor;
    int old = capacity;
    capacity = capacity * 2;
    *editorPtr = realloc(*editorPtr, sizeof(char*) * capacity);
    editorSetNull(old, capacity);
}

void change(int pInd1, int pInd2) {
    int size = pInd2 - pInd1 + 1;
    char* toAdd;
    char temp[MAXSIZE];
    int ctr = 0;

    char** lines = malloc(sizeof(char*) * size);
    int last;

    while(capacity <= pInd2) {
        editorExpand();
    }

    for(int i = pInd1; i < (pInd2+1); i++) {
        fgets(temp, MAXSIZE, stdin);
        int len = strlen(temp);
        toAdd = malloc(sizeof(char) * (len+1));
        strncpy(toAdd, temp, len);
        toAdd[len-1] = '\0';
        editor[i] = toAdd;
        lines[ctr] = toAdd;
        ctr++;
    }
    fgets(temp, MAXSIZE, stdin);

    if(lastLine < pInd2) {
        lastLine = pInd2;
    }
    last = lastLine;

    push(stack, pInd1, pInd2, 1, lines, last, lastCheckpoint, capacity);
}

void delete(int pInd1, int pInd2) {
    cleanDeleteIndexes(&pInd1, &pInd2);

    if(pInd1 == 0 && pInd2 == 0) {
        // do nothing
    } else {
        int size = pInd2 - pInd1 + 1;
        int ptr = 0;
        for(int i = pInd1; i < pInd2+1; i++) {
            editor[i] = NULL;
            ptr++;
        }
        int first = pInd1;
        int second = pInd2+1;
        while(editor[second]) {
            editor[first] = editor[second];
            editor[second] = NULL;
            first++;
            second++;
        }
        lastLine = lastLine - size;
    }

    char** edPost = malloc(sizeof(char*) * capacity);
    int last = lastLine;
    for(int i = 0; i < capacity; i++) {
        edPost[i] = editor[i];
    }

    lastCheckpoint = stack->undoSSize + 1;
    push(stack, pInd1, pInd2, 2, edPost, last, lastCheckpoint, capacity);
}

void cleanDeleteIndexes(int* pInd1, int* pInd2) {
    if(*pInd1 == 0 && *pInd2 != 0) {
        *pInd1 = 1;
    }
    if(*pInd1 > lastLine) {
        *pInd1 = 0;
        *pInd2 = 0;
    }
    if(*pInd2 > lastLine) {
        *pInd2 = lastLine;
    }
}

void print(int pInd1, int pInd2) {
    int extraLines = 0;
    if(pInd1 == 0 && pInd2 == 0) {
        puts(".");
        return;
    }
    if(pInd2 > lastLine) {
        extraLines = pInd2 - lastLine;
        pInd2 = lastLine;
    }
    for(int i = pInd1; i < pInd2+1; i++) {
        puts(editor[i]);
    }
    for(int i = 0; i < extraLines; i++) {
        puts(".");
    }
}

void queueUndo(int ind) {
    qUndo = qUndo + ind;
    int check = stack->undoSSize - qUndo + qRedo;
    if(check < 0) {
        qUndo = stack->undoSSize + qRedo;
    }
}

void queueRedo(int ind) {
    qRedo = qRedo + ind;
    int check = redoSSize  - qRedo + qUndo;
    if(check < 0) {
        qRedo = redoSSize + qUndo;
    }
}

void undo(int ind) {
    int target = stack->undoSSize - ind;

    if((stack->arr)[target].opcode == 2) {
        restoreCheckpoint(target);

        char** newEditor = malloc(sizeof(char*) * capacity);

        for(int i = 0; i < stack->arr[(stack->arr)[target].checkpoint].savedcap; i++) {
            newEditor[i] = editor[i];
        }

        for(int i = stack->arr[(stack->arr)[target].checkpoint].savedcap; i < capacity; i++) {
            newEditor[i] = NULL;
        }

        editor = newEditor;
    }

    if((stack->arr)[target].opcode == 1) {
        restoreCheckpoint((stack->arr)[target].checkpoint);

        char** newEditor = malloc(sizeof(char*) * capacity);

        for(int i = 0; i < stack->arr[(stack->arr)[target].checkpoint].savedcap; i++) {
            newEditor[i] = editor[i];
        }

        for(int i = stack->arr[(stack->arr)[target].checkpoint].savedcap; i < capacity; i++) {
            newEditor[i] = NULL;
        }
        editor = newEditor;

        int diff = target - (stack->arr)[target].checkpoint;

        for(int i = 1; i < diff+1; i++) {
            doChange(newEditor, (stack->arr)[target].checkpoint + i);
        }
        editor = newEditor;
    }
    stack->undoSSize = stack->undoSSize - ind;
    redoSSize = redoSSize + ind;
}

void redo(int ind) {
    int target = stack->undoSSize + ind;

    if((stack->arr)[target].opcode == 2) {
        restoreCheckpoint(target);

        char** newEditor = malloc(sizeof(char*) * capacity);

        for(int i = 0; i < stack->arr[(stack->arr)[target].checkpoint].savedcap; i++) {
            newEditor[i] = editor[i];
        }

        for(int i = stack->arr[(stack->arr)[target].checkpoint].savedcap; i < capacity; i++) {
            newEditor[i] = NULL;
        }
        editor = newEditor;
    }

    if((stack->arr)[target].opcode == 1) {
        restoreCheckpoint((stack->arr)[target].checkpoint);

        char** newEditor = malloc(sizeof(char*) * capacity);

        for(int i = 0; i < stack->arr[(stack->arr)[target].checkpoint].savedcap; i++) {
            newEditor[i] = editor[i];
        }

        for(int i = stack->arr[(stack->arr)[target].checkpoint].savedcap; i < capacity; i++) {
            newEditor[i] = NULL;
        }
        editor = newEditor;

        int diff = target - (stack->arr)[target].checkpoint;

        for(int i = 1; i < diff+1; i++) {
            doChange(newEditor, (stack->arr)[target].checkpoint + i);
        }
        editor = newEditor;
    }

    stack->undoSSize= stack->undoSSize + ind;
    redoSSize = redoSSize - ind;
}

void restoreCheckpoint(int checkpoint) {
    struct Operation opCheckpoint = (stack->arr)[checkpoint];
    char** oldEditor = editor;
    int oldLast = lastLine;
    editor = opCheckpoint.saved;
    lastLine = opCheckpoint.last;
}

void doChange(char** edd, int ind) {
    struct Operation op = ((stack->arr)[ind]);
    int ind1 = op.ind1;
    int ind2 = op.ind2;
    int ptr = 0;

    for(int i = ind1; i < ind2+1; i++) {
        edd[i] = (op.saved)[ptr];
        ptr++;
    }

    if(lastLine < ind2) {
        lastLine = ind2;
    }
}

void fixCheckpoint(int* checkpoint) {
    int start = stack->undoSSize;
    for(int i = start; i > -1; i--) {
        if( ((stack->arr)[i]).opcode == 2) {
            lastCheckpoint = i;
            break;
        }
    }
}



struct Stack* createStack(int cap) {
    struct Stack* s = malloc(sizeof(struct Stack) * cap);
    s->arr = malloc(sizeof(struct Operation) * cap);
    s->undoSSize = -1;
    s->cap = cap;
    s->size = 0;
    return s;
}

void doubleStack(struct Stack* s) {
    s->cap = s->cap * 2;
    s->arr = realloc(s->arr, sizeof(struct Operation) * s->cap);
    if(s->arr == NULL) {
        exit(1);
    }
}

int isFull(struct Stack* s) {
    return s->size == s->cap;
}

void push(struct Stack* s,  int pInd1, int pInd2, int pOpcode, char** pSaved, int pLast, int pCheckpoint, int pSavedcap) {
    if(isFull(s)) {
        doubleStack(s);
    }
    s->undoSSize++;
    (s->arr[(s->undoSSize)]).ind1 = pInd1;
    (s->arr[(s->undoSSize)]).ind2 = pInd2;
    (s->arr[(s->undoSSize)]).opcode = pOpcode;
    (s->arr[(s->undoSSize)]).saved = pSaved;
    (s->arr[(s->undoSSize)]).last = pLast;
    (s->arr[(s->undoSSize)]).checkpoint = pCheckpoint;
    (s->arr[(s->undoSSize)]).savedcap = pSavedcap;
    s->size++;
}


