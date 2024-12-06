 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>

#define MAX_LINE 2048

typedef struct {
    char county[100];
    char state[3];
    float education_high_school_or_higher;
    float education_bachelors_or_higher;
    float ethnicities_american_indian_alaska_native;
    float ethnicities_asian;
    float ethnicities_black;
    float ethnicities_hispanic_or_latino;
    float ethnicities_hawaiian;
    float ethnicities_two_or_more;
    float ethnicities_white;
    float ethnicities_white_not_hispanic;
    int income_median_household;
    int income_per_capita;
    float income_persons_below_poverty;
    int population_2014;
} Demographics;


void remove_quotes(char *str) {
    // REMOVE THE EXCESS QUOTES FROM THE CSV FILE
    size_t len = strlen(str);
    if (str[0] == '"' && str[len - 1] == '"') {
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }
}

void display(int record_count, Demographics counties[]);
int population(int record_count, Demographics counties[], char* type);

 int main(int argc, char const *argv[])
 {
    // VALIDITY OF PARAMETERS
    if (argc != 3) {
        fprintf(stderr, "ERROR: expected ./demographics_parse <csv file> <ops file>\n");
        return 1;
    }

    // PARAMTER 1: ENDS IN ".csv"
    if (!strstr(argv[1], ".csv") || strcmp(argv[1] + strlen(argv[1]) - 4, ".csv") != 0) {
        fprintf(stderr, "ERROR: expected first file to end in .csv\n");
        return 1;
    }

    // PARAMTER 2: ENDS IN ".ops"
    if (!strstr(argv[2], ".ops") || strcmp(argv[2] + strlen(argv[2]) - 4, ".ops") != 0) {
        fprintf(stderr, "ERROR: expected second file to end in .ops\n");
        return 1;
    }

    // PARSE DEMOGRAPHICS FILE
    FILE *csv = fopen(argv[1], "r");
    if (!csv) {
        fprintf(stderr, "ERROR: Could not open file %s\n", argv[1]);
        return 1;
    }

    int record_count = 0;
    char line[MAX_LINE];
    Demographics counties[4000];

    // SKIP THE HEADER
    if (fgets(line, MAX_LINE, csv) == NULL) {
        fprintf(stderr, "ERROR: File is empty or invalid format\n");
        fclose(csv);
        exit(1);
    }

    // READ UNTIL EOF
    while (fgets(line, MAX_LINE, csv) != NULL) {
        // printf("UNPROCESSED LINE: %s\n", line);
        char *token = strtok(line, ",");
        // COUNTY
        remove_quotes(token);
        strncpy(counties[record_count].county, token, sizeof(counties[record_count].county) - 1);
        counties[record_count].county[sizeof(counties[record_count].county) - 1] = '\0';  // NULL TERMINATION

        // STATE
        token = strtok(NULL, ",");
        if (token == NULL) {
            fprintf(stderr, "ERROR: Unexpected format in CSV at line %d\n", record_count + 1);
            continue; // Skip this line and continue
        }
        remove_quotes(token);
        strncpy(counties[record_count].state, token, sizeof(counties[record_count].state) - 1);
        counties[record_count].state[sizeof(counties[record_count].state) - 1] = '\0';  // NULL TERMINATION
        
        // SKIP: TO EDUCATION
        for (int i = 0; i < 3; ++i) {
            token = strtok(NULL, ",");
            if (token == NULL) {
                fprintf(stderr, "ERROR: Unexpected format in CSV at line %d\n", record_count + 1);
                continue;
            }
        }

        // Education.High School or Higher
        token = strtok(NULL, ",");
        if (token != NULL) {
            remove_quotes(token);
            counties[record_count].education_high_school_or_higher = atof(token);
        }

        // Education.Bachelor's Degree or Higher
        token = strtok(NULL, ",");
        if (token != NULL) {
            remove_quotes(token);
            counties[record_count].education_bachelors_or_higher = atof(token);
        }

        // SKIP: TO ETHNICITIES
        for (int i = 0; i < 4; ++i) {
            token = strtok(NULL, ",");
            if (token == NULL) {
                fprintf(stderr, "ERROR: Unexpected format in CSV at line %d\n", record_count + 1);
                continue;
            }
        }

        // Ethnicities.American Indian and Alaska Native Alone
        token = strtok(NULL, ",");
        if (token != NULL) {
            remove_quotes(token);
            counties[record_count].ethnicities_american_indian_alaska_native = atof(token);
        }

        // Ethnicities.Asian Alone
        token = strtok(NULL, ",");
        if (token != NULL) {
            remove_quotes(token);
            counties[record_count].ethnicities_asian = atof(token);
        }

        // Ethnicities.Black Alone
        token = strtok(NULL, ",");
        if (token != NULL) {
            remove_quotes(token);
            counties[record_count].ethnicities_black = atof(token);
        }

        // Ethnicities.Hispanic or Latino
        token = strtok(NULL, ",");
        if (token != NULL) {
            remove_quotes(token);
            counties[record_count].ethnicities_hispanic_or_latino = atof(token);
        }

        // Ethnicities.Native Hawaiian and Other Pacific Islander Alone
        token = strtok(NULL, ",");
        if (token != NULL) {
            remove_quotes(token);
            counties[record_count].ethnicities_hawaiian = atof(token);
        }

        // Ethnicities.Two or More Races
        token = strtok(NULL, ",");
        if (token != NULL) {
            remove_quotes(token);
            counties[record_count].ethnicities_two_or_more = atof(token);
        }

        // Ethnicities.White Alone
        token = strtok(NULL, ",");
        if (token != NULL) {
            remove_quotes(token);
            counties[record_count].ethnicities_white = atof(token);
        }

        // Ethnicities.White Alone not Hispanic or Latino
        token = strtok(NULL, ",");
        if (token != NULL) {
            remove_quotes(token);
            counties[record_count].ethnicities_white_not_hispanic = atof(token);
        }

        // SKIP: TO INCOME
        for (int i = 0; i < 6; ++i) {
            token = strtok(NULL, ",");
            if (token == NULL) {
                fprintf(stderr, "ERROR: Unexpected format in CSV at line %d\n", record_count + 1);
                continue;
            }
        }

        // Income.Median Household Income
        token = strtok(NULL, ",");
        if (token != NULL) {
            remove_quotes(token);
            counties[record_count].income_median_household = atoi(token);
        }

        // Income.Per Capita Income
        token = strtok(NULL, ",");
        if (token != NULL) {
            remove_quotes(token);
            counties[record_count].income_per_capita = atoi(token);
        }

        // Income.Persons Below Poverty Level
        token = strtok(NULL, ",");
        if (token != NULL) {
            remove_quotes(token);
            counties[record_count].income_persons_below_poverty = atof(token);
        }

        // SKIP: TO POPULATION
        for (int i = 0; i < 10; ++i) {
            token = strtok(NULL, ",");
            if (token == NULL) {
                fprintf(stderr, "ERROR: Unexpected format in CSV at line %d\n", record_count + 1);
                continue;
            }
        }

        // Population.2014 Population
        token = strtok(NULL, ",");
        if (token != NULL) {
            remove_quotes(token);
            counties[record_count].population_2014 = atoi(token);
        }

        record_count++;
        if (record_count >= 4000) {
            fprintf(stderr, "ERROR: Maximum record count reached\n");
            break;
        }
    }
    fclose(csv);
    printf("%d records loaded\n", record_count);
}
 