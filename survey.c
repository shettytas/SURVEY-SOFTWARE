#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "survey.h"

// ================================================================
//  Utility Function: clearStdin
//  Purpose: Clears input buffer to avoid unwanted inputs in scanf/fgets
//  Complexity: O(k), where k is number of characters in stdin
// ================================================================
void clearStdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// ================================================================
//  BST SECTION - Handles all response storage and analysis
// ================================================================

// ------------------------------------------------
// Function: createBSTNode
// Description: Creates a new BST node with given option text
// Complexity: O(1)
// ------------------------------------------------
BSTNode* createBSTNode(char *option) {
    BSTNode *node = (BSTNode*)malloc(sizeof(BSTNode));
    if (!node) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strncpy(node->option, option, sizeof(node->option) - 1);
    node->option[sizeof(node->option) - 1] = '\0';
    node->count = 1;
    node->left = node->right = NULL;
    return node;
}

// ------------------------------------------------
// Function: insertBST
// Description: Inserts an option or increments its count in the BST
// Complexity: Average O(log n), Worst O(n)
// ------------------------------------------------
BSTNode* insertBST(BSTNode *root, char *option) {
    if (!root) return createBSTNode(option);
    int cmp = strcmp(option, root->option);
    if (cmp == 0)
        root->count++;
    else if (cmp < 0)
        root->left = insertBST(root->left, option);
    else
        root->right = insertBST(root->right, option);
    return root;
}

// ------------------------------------------------
// Function: searchBST
// Description: Finds and returns a node matching the option text
// Complexity: Average O(log n), Worst O(n)
// ------------------------------------------------
BSTNode* searchBST(BSTNode *root, const char *key) {
    if (!root) return NULL;
    int cmp = strcmp(key, root->option);
    if (cmp == 0) return root;
    else if (cmp < 0) return searchBST(root->left, key);
    else return searchBST(root->right, key);
}

// ------------------------------------------------
// Function: totalResponsesBST
// Description: Returns total responses stored in a question's BST
// Complexity: O(n) where n = number of distinct options
// ------------------------------------------------
int totalResponsesBST(BSTNode *root) {
    if (!root) return 0;
    return root->count + totalResponsesBST(root->left) + totalResponsesBST(root->right);
}

// ================================================================
//  QUESTION / SURVEY SECTION - Linked List-based survey management
// ================================================================

// ------------------------------------------------
// Function: newQuestion
// Description: Allocates and initializes a new Question node
// Complexity: O(1)
// ------------------------------------------------
Question* newQuestion(char *text, int numOptions) {
    Question *q = (Question*)malloc(sizeof(Question));
    if (!q) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strncpy(q->text, text, sizeof(q->text) - 1);
    q->text[sizeof(q->text) - 1] = '\0';
    q->numOptions = numOptions;
    q->responses = NULL;
    q->totalResponses = 0;
    q->next = NULL;
    return q;
}

// ------------------------------------------------
// Function: createSurveyNode
// Description: Creates a new Survey node with given title
// Complexity: O(1)
// ------------------------------------------------
SurveyNode* createSurveyNode(char *title) {
    SurveyNode *s = (SurveyNode*)malloc(sizeof(SurveyNode));
    if (!s) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strncpy(s->title, title, sizeof(s->title) - 1);
    s->title[sizeof(s->title) - 1] = '\0';
    s->questions = NULL;
    s->conducted = 0;
    s->next = NULL;
    return s;
}

// ------------------------------------------------
// Function: addQuestionToSurvey
// Description: Adds multiple questions (based on user input count) to a survey
// Complexity: O(q * o), where q = number of questions, o = number of options per question
// ------------------------------------------------
void addQuestionToSurvey(SurveyNode *s) {
    if (!s) {
        printf("Invalid survey pointer.\n");
        return;
    }

    int qCount;
    printf("Enter number of questions to add: ");
    if (scanf("%d", &qCount) != 1 || qCount <= 0) {
        printf("Invalid input. \n");
        clearStdin();
        return;
    }
    clearStdin();

    for (int qn = 1; qn <= qCount; ++qn) {
        char text[200];
        int nopt;

        printf("\nEnter text for Question %d: ", qn);
        if (!fgets(text, sizeof(text), stdin)) return;
        text[strcspn(text, "\n")] = '\0';
        if (strlen(text) == 0) {
            printf("Question cannot be empty. Skipping.\n");
            continue;
        }

        printf("Enter number of options (2-5): ");
        if (scanf("%d", &nopt) != 1 || nopt < 2 || nopt > 5) {
            printf("Invalid option count. Skipping question.\n");
            clearStdin();
            continue;
        }
        clearStdin();

        Question *q = newQuestion(text, nopt);
        for (int i = 0; i < nopt; ++i) {
            printf("Option %d: ", i + 1);
            if (!fgets(q->options[i], sizeof(q->options[i]), stdin))
                q->options[i][0] = '\0';
            q->options[i][strcspn(q->options[i], "\n")] = '\0';
            if (strlen(q->options[i]) == 0)
                strcpy(q->options[i], "(empty option)");
        }

        // Append question to linked list
        if (!s->questions) s->questions = q;
        else {
            Question *t = s->questions;
            while (t->next) t = t->next;
            t->next = q;
        }
    }
    printf("\nAll questions added successfully to survey: %s\n", s->title);
}

// ------------------------------------------------
// Function: addQuestion
// Description: Creates a new survey and adds it to list
// Complexity: O(1)
// ------------------------------------------------
void addQuestion(SurveyNode **head) {
    char title[100];
    printf("Enter Survey Title: ");
    clearStdin();
    if (!fgets(title, sizeof(title), stdin)) return;
    title[strcspn(title, "\n")] = '\0';
    if (strlen(title) == 0) {
        printf("Survey title cannot be empty.\n");
        return;
    }

    SurveyNode *node = createSurveyNode(title);
    node->next = *head;
    *head = node;

    printf("Survey \"%s\" created successfully!\n", title);
}

// ------------------------------------------------
// Function: viewSurveyDetails
// Description: Displays all questions and options of a selected survey
// Complexity: O(q * o)
// ------------------------------------------------
void viewSurveyDetails(SurveyNode *head) {
    SurveyNode *s = selectAnySurvey(head);
    if (!s) return;

    printf("\nSurvey Title: %s\n", s->title);
    if (!s->questions) {
        printf("No questions added yet.\n");
        return;
    }

    int qno = 1;
    for (Question *q = s->questions; q; q = q->next, ++qno) {
        printf("\nQ%d: %s\n", qno, q->text);
        for (int i = 0; i < q->numOptions; ++i)
            printf("  %d. %s\n", i + 1, q->options[i]);
    }
    printf("\nEnd of survey details.\n");
}

// ------------------------------------------------
// Function: selectSurveyWithQuestions
// Description: Displays and allows selection among surveys that contain questions
// Complexity: O(n)
// ------------------------------------------------
SurveyNode* selectSurveyWithQuestions(SurveyNode *head) {
    if (!head) {
        printf("No surveys available.\n");
        return NULL;
    }
    int i = 0;
    for (SurveyNode *t = head; t; t = t->next)
        if (t->questions) printf("%d. %s\n", ++i, t->title);
    if (i == 0) {
        printf("No surveys with questions.\n");
        return NULL;
    }

    int choice;
    printf("Select survey number: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        clearStdin();
        return NULL;
    }

    int idx = 0;
    for (SurveyNode *t = head; t; t = t->next)
        if (t->questions && ++idx == choice)
            return t;
    printf("Invalid selection.\n");
    return NULL;
}

// ------------------------------------------------
// Function: selectSurveyConducted
// Description: Displays and allows selection among surveys that were conducted
// Complexity: O(n)
// ------------------------------------------------
SurveyNode* selectSurveyConducted(SurveyNode *head) {
    if (!head) {
        printf("No surveys available.\n");
        return NULL;
    }
    int i = 0;
    for (SurveyNode *t = head; t; t = t->next)
        if (t->conducted) printf("%d. %s\n", ++i, t->title);
    if (i == 0) {
        printf("No surveys have been conducted yet.\n");
        return NULL;
    }

    int choice;
    printf("Select survey number: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        clearStdin();
        return NULL;
    }

    int idx = 0;
    for (SurveyNode *t = head; t; t = t->next)
        if (t->conducted && ++idx == choice)
            return t;
    printf("Invalid selection.\n");
    return NULL;
}

// ------------------------------------------------
// Function: selectAnySurvey
// Description: Displays all surveys and allows user to select one
// Complexity: O(n)
// ------------------------------------------------
SurveyNode* selectAnySurvey(SurveyNode *head) {
    if (!head) {
        printf("No surveys available.\n");
        return NULL;
    }

    int i = 1;
    for (SurveyNode *t = head; t; t = t->next, ++i)
        printf("%d. %s\n", i, t->title);

    int choice;
    printf("Enter survey number: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        clearStdin();
        return NULL;
    }

    int idx = 1;
    for (SurveyNode *t = head; t; t = t->next, ++idx)
        if (idx == choice) return t;
    printf("Invalid selection.\n");
    return NULL;
}

// ================================================================
//  Conduct and Publish Survey
// ================================================================

// ------------------------------------------------
// Function: conductSurvey
// Description: Records user responses for all questions in a survey
// Complexity: O(q * log m)
// ------------------------------------------------
void conductSurvey(SurveyNode *head) {
    SurveyNode *s = selectSurveyWithQuestions(head);
    if (!s) return;

    printf("\nConducting survey: %s\n", s->title);
    for (Question *q = s->questions; q; q = q->next) {
        printf("\nQ: %s\n", q->text);
        for (int i = 0; i < q->numOptions; ++i)
            printf("%d. %s\n", i + 1, q->options[i]);

        int choice;
        printf("Enter your choice (1-%d): ", q->numOptions);
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > q->numOptions) {
            printf("Invalid input. Skipping question.\n");
            clearStdin();
            break;
        }

        q->responses = insertBST(q->responses, q->options[choice - 1]);
        q->totalResponses++;
    }
    s->conducted = 1;
    printf("\nResponses recorded successfully for: %s\n", s->title);
}

// ------------------------------------------------
// Function: publishResults
// Description: Displays analyzed survey results with all option counts
// Complexity: O(q * m)
// ------------------------------------------------
void publishResults(SurveyNode *head) {
    SurveyNode *s = selectSurveyConducted(head);
    if (!s) return;

    printf("\n====== Results for \"%s\" ======\n", s->title);
    int qno = 1;

    for (Question *q = s->questions; q; q = q->next, ++qno) {
        printf("\nQ%d: %s\n", qno, q->text);
        int total = totalResponsesBST(q->responses);

        if (total == 0) {
            printf("No responses recorded.\n");
            continue;
        }

        for (int i = 0; i < q->numOptions; i++) {
            BSTNode *node = searchBST(q->responses, q->options[i]);
            int count = node ? node->count : 0;
            float pct = (total == 0) ? 0 : (count * 100.0f / total);

            printf("%-20s : %2d (%.1f%%) ", q->options[i], count, pct);
            for (int j = 0; j < pct / 5; j++) printf("#");
            printf("\n");
        }
    }

    printf("\nEnd of results for: %s\n", s->title);
}
