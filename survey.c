#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "survey.h"

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
    // Loop until valid number of questions
    while (1) {
        printf("Enter number of questions to add: ");
        if (scanf("%d", &qCount) != 1 || qCount <= 0) {
            printf("Invalid input. Please enter a positive number.\n");
            scanf("%*[^\n]"); // consume invalid input
            scanf("%*c");     // consume newline
        } else {
            scanf("%*c"); // consume newline after valid number
            break;
        }
    }

    for (int qn = 1; qn <= qCount; ++qn) {
        char text[200];
        int nopt;

        // Loop until non-empty question text
        while (1) {
            printf("\nEnter text for Question %d: ", qn);
            if (scanf("%199[^\n]%*c", text) != 1) {
                printf("Question cannot be empty. Please enter again.\n");
                continue;
            }
            if (strlen(text) == 0) {
                printf("Question cannot be empty. Please enter again.\n");
            } else break;
        }

        // Loop until valid number of options
        while (1) {
            printf("Enter number of options (2-5): ");
            if (scanf("%d", &nopt) != 1 || nopt < 2 || nopt > 5) {
                printf("Invalid input. Number must be between 2 and 5.\n");
                scanf("%*[^\n]"); // consume invalid input
                scanf("%*c");     // consume newline
            } else {
                scanf("%*c"); // consume newline after valid number
                break;
            }
        }

        Question *q = newQuestion(text, nopt);

        // Loop for each option, force non-empty input
        for (int i = 0; i < nopt; ++i) {
            while (1) {
                printf("Option %d: ", i + 1);
                if (scanf("%49[^\n]%*c", q->options[i]) != 1) {
                    printf("Option cannot be empty. Please enter again.\n");
                    continue;
                }
                if (strlen(q->options[i]) == 0) {
                    printf("Option cannot be empty. Please enter again.\n");
                } else break;
            }
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
// Function: addSurveyName
// Description: Creates a new survey title and adds it to list
// Complexity: O(1)
// ------------------------------------------------
void addSurveyName(SurveyNode **head) {
    char title[100];

    while (1) {
        printf("Enter Survey Title: ");
        if (scanf("%99[^\n]%*c", title) != 1) {
            printf("Survey title cannot be empty.\n");
            continue;
        }

        // Check if title contains at least ONE visible character
        int allSpaces = 1;
        for (int i = 0; title[i] != '\0'; i++) {
            if (title[i] != ' ' && title[i] != '\t') { 
                allSpaces = 0;
                break;
            }
        }

        if (allSpaces) {
            printf("Survey title cannot be empty.\n");
            continue;
        }

        // Check duplicate
        int duplicate = 0;
        for (SurveyNode *temp = *head; temp; temp = temp->next) {
            if (strcmp(temp->title, title) == 0) {
                duplicate = 1;
                break;
            }
        }

        if (duplicate) {
            printf("A survey named \"%s\" already exists. Please enter a different title.\n", title);
            continue;
        }

        // Create new survey
        SurveyNode *node = createSurveyNode(title);
        node->next = *head;
        *head = node;

        printf("Survey \"%s\" created successfully!\n", title);
        break;
    }
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
        scanf("%*[^\n]");
        scanf("%*c");
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
        scanf("%*[^\n]");
        scanf("%*c");
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
        scanf("%*[^\n]");
        scanf("%*c");
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
            scanf("%*[^\n]");
            scanf("%*c");
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
            // Print bar graph using '#'
            for (int j = 0; j < pct / 5; j++)
                printf("#");

            printf("\n");       
        }
    }

    printf("\nEnd of results for: %s\n", s->title);
}

// ------------------------------------------------
// Function: deleteQuestion
// Description: Displays all questions with options, then deletes selected question
// Complexity: O(q * o)
// ------------------------------------------------
void deleteQuestion(SurveyNode *head) {
    SurveyNode *s = selectAnySurvey(head);
    if (!s) return;

    if (!s->questions) {
        printf("No questions in this survey to delete.\n");
        return;
    }

    printf("\nSurvey Title: %s\n", s->title);
    printf("Current Questions:\n");
    
    // Display all questions with their options
    int qno = 1;
    for (Question *q = s->questions; q; q = q->next, ++qno) {
        printf("\nQ%d: %s\n", qno, q->text);
        for (int i = 0; i < q->numOptions; ++i)
            printf("  %d. %s\n", i + 1, q->options[i]);
    }

    // Get user choice
    int choice;
    printf("\nEnter question number to delete (1-%d): ", qno - 1);
    if (scanf("%d", &choice) != 1 || choice < 1 || choice >= qno) {
        printf("Invalid choice.\n");
        scanf("%*[^\n]");
        scanf("%*c");
        return;
    }
    scanf("%*c");

    // Delete the question
    Question *prev = NULL;
    Question *curr = s->questions;
    int idx = 1;

    while (curr && idx < choice) {
        prev = curr;
        curr = curr->next;
        idx++;
    }

    if (!curr) {
        printf("Question not found.\n");
        return;
    }

    // Remove from linked list
    if (prev == NULL) {
        s->questions = curr->next;
    } else {
        prev->next = curr->next;
    }

    // Free the question node
    free(curr);
    printf("Question %d deleted successfully!\n", choice);
}

// ------------------------------------------------
// Function: deleteSurvey
// Description: Deletes the entire selected survey
// Complexity: O(n)
// ------------------------------------------------
void deleteSurvey(SurveyNode **head) {
    if (!*head) {
        printf("No surveys available to delete.\n");
        return;
    }

    SurveyNode *s = selectAnySurvey(*head);
    if (!s) return;

    // Save the title before freeing
    char titleCopy[100];
    strncpy(titleCopy, s->title, sizeof(titleCopy) - 1);
    titleCopy[sizeof(titleCopy) - 1] = '\0';

    // Find and remove the survey
    SurveyNode *prev = NULL;
    SurveyNode *curr = *head;

    while (curr && curr != s) {
        prev = curr;
        curr = curr->next;
    }

    if (!curr) {
        printf("Survey not found.\n");
        return;
    }

    // Remove from linked list
    if (prev == NULL) {
        *head = curr->next;
    } else {
        prev->next = curr->next;
    }

    // Free all questions in the survey
    Question *q = curr->questions;
    while (q) {
        Question *temp = q;
        q = q->next;
        free(temp);
    }

    // Free the survey node
    free(curr);
    printf("Survey \"%s\" deleted successfully!\n", titleCopy);
}