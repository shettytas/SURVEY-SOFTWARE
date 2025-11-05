#include <stdio.h>
#include <stdlib.h>
#include "survey.h"

int main() {
    SurveyNode *surveyList = NULL;
    int choice;

    while (1) {
        printf("\n=== SURVEY SOFTWARE ===\n");
        printf("1. Create new survey\n");
        printf("2. Add question to existing survey\n");
        printf("3. View survey and all its questions\n");
        printf("4. Conduct a survey\n");
        printf("5. Publish survey results\n");
        printf("6. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Enter number between 1–6.\n");
            clearStdin();
            continue;
        }

        switch (choice) {
            case 1:
                addQuestion(&surveyList);
                break;
            case 2: {
                SurveyNode *s = selectAnySurvey(surveyList);
                if (s) addQuestionToSurvey(s);
                break;
            }
            case 3:
                viewSurveyDetails(surveyList);
                break;
            case 4:
                conductSurvey(surveyList);
                break;
            case 5:
                publishResults(surveyList);
                break;
            case 6:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. \n");
        }
    }
}
