#ifndef SURVEY_H
#define SURVEY_H

// ------------------------- STRUCTURE DEFINITIONS -------------------------

// Binary Search Tree node for storing survey responses
typedef struct BSTNode {
    char option[50];        // option text (e.g., "Agree")
    int count;              // frequency of this option
    struct BSTNode *left, *right;
} BSTNode;

// Linked list node for storing questions
typedef struct Question {
    char text[200];         // question text
    char options[5][50];    // up to 5 options
    int numOptions;         // number of options
    BSTNode *responses;     // root of BST that holds responses for this question
    int totalResponses;     // total responses recorded for this question
    struct Question *next;
} Question;

// Survey containing linked list of questions
typedef struct Survey {
    char title[100];
    Question *questions;    // head of question linked list
    int conducted;          // flag: 0 = never conducted, 1 = conducted at least once
    struct Survey *next;
} Survey;

// Linked list head type alias
typedef Survey SurveyNode;

// ------------------------- FUNCTION PROTOTYPES -------------------------

// BST operations
BSTNode* createBSTNode(char *option);
BSTNode* insertBST(BSTNode *root, char *option);
BSTNode* searchBST(BSTNode *root, const char *key);
int totalResponsesBST(BSTNode *root);

// Question and survey operations
Question* newQuestion(char *text, int numOptions);
void addQuestion(SurveyNode **head);           
void addQuestionToSurvey(SurveyNode *s); 
void viewSurveyDetails(SurveyNode *head);      
void conductSurvey(SurveyNode *head);          
void publishResults(SurveyNode *head);         

// Multiple surveys operations
SurveyNode* createSurveyNode(char *title);
SurveyNode* selectSurveyWithQuestions(SurveyNode *head);   
SurveyNode* selectSurveyConducted(SurveyNode *head);      
SurveyNode* selectAnySurvey(SurveyNode *head);            

// Utility
void clearStdin();

#endif
