 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <math.h>

#define MAX_LINE 2048

typedef struct {
    char county[100];
    char state[3];
    double education_high_school_or_higher;
    double education_bachelors_or_higher;
    double ethnicities_american_indian_alaska_native;
    double ethnicities_asian;
    double ethnicities_black;
    double ethnicities_hispanic_or_latino;
    double ethnicities_hawaiian;
    double ethnicities_two_or_more;
    double ethnicities_white;
    double ethnicities_white_not_hispanic;
    int income_median_household;
    int income_per_capita;
    double income_persons_below_poverty;
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
double population(int record_count, Demographics counties[], char* type);
Demographics* filter_state(int *record_count, Demographics counties[], char* state); 
Demographics* filter_numeric(int *record_count, Demographics counties[], char* field, char *operation, float number);

int main(int argc, char const *argv[]) {
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
    Demographics* counties = (Demographics*)malloc(4000 * sizeof(Demographics));

    // SKIP THE HEADER
    if (fgets(line, MAX_LINE, csv) == NULL) {
        fprintf(stderr, "ERROR: File is empty or invalid format\n");
        fclose(csv);
        return 1;
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
            continue;
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

        // Education.Bachelor's Degree or Higher
        token = strtok(NULL, ",");
        if (token != NULL) {
            remove_quotes(token);
            counties[record_count].education_bachelors_or_higher = atof(token);
        }

        // Education.High School or Higher
        token = strtok(NULL, ",");
        if (token != NULL) {
            remove_quotes(token);
            counties[record_count]. education_high_school_or_higher= atof(token);
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

    // PARSE OPERATION FILE
    FILE *ops = fopen(argv[2], "r");
    if (!ops) {
        fprintf(stderr, "ERROR: Could not open file %s\n", argv[2]);
        return 1;
    }

    // READ UNTIL EOF
    while (fgets(line, MAX_LINE, ops) != NULL) {
        //printf("line: %s", line);
        // REMOVE NEW LINE CHAR (IF APPLICIABLE)
        line[strcspn(line, "\n")] = '\0';
        
        // NON INCLUSIVE ':'
        if (strcmp(line, "display") == 0) {
            display(record_count, counties);
        } else if (strcmp(line, "population-total") == 0) {
            printf("2014 population: %.0f\n", population(record_count, counties, "total"));
        } else {
            char command[128];
            char parameters[128];
            char *colon_position = strchr(line, ':');

            if (colon_position != NULL) {
                // COPY THE COMMAND
                strncpy(command, line, colon_position - line);
                command[colon_position - line] = '\0';  // NULL TERMINATION

                // COPY ITS PARAMETERS
                strcpy(parameters, colon_position + 1);

                // RUN OPERATIONS (filter, filter-state, population, percent)
                // POPULATION
                if (strcmp(command, "population") == 0) {
                    //printf("parameter: %s\n",parameters);
                    printf("2014 %s population %f\n", parameters, population(record_count, counties, parameters));
                } else if (strcmp(command, "percent") == 0) {
                    // 100 * (filtered total/total)
                    double total = population(record_count, counties, "total");
                    double filtered = population(record_count, counties, parameters);
                    printf("2014 %s percentage %f\n", parameters, 100 * (filtered/total));
                } else if (strcmp(command, "filter-state") == 0) {
                    Demographics *filtered = filter_state(&record_count, counties, parameters);

                    if (filtered != NULL) {
                        free(counties);
                        counties = filtered;
                        free(filtered);
                    }

                    printf("Filter: state == %s (%d entries)\n", parameters, record_count);
                } else if (strcmp(command, "filter") == 0) {
                    // PARSE REST OF PARAMETERS
                    // type, ge/le (greater than or equal or less than or equal), number
                    char filter_field[128];
                    char filter_ge_le[128];
                    float filter_number;
                    char *filtered_colon_position = strchr(parameters, ':');

                    if (filtered_colon_position != NULL) {
                        strncpy(filter_field, parameters, filtered_colon_position - parameters);
                        filter_field[filtered_colon_position - parameters] = '\0';  // NULL TERMINATION

                        char *rest = filtered_colon_position + 1;

                        filtered_colon_position = strchr(rest, ':');

                        if (filtered_colon_position != NULL) {
                            strncpy(filter_ge_le, rest, filtered_colon_position - rest);
                            filter_ge_le[filtered_colon_position - rest] = '\0';  // NULL TERMINATION

                            char number_part[128];
                            strcpy(number_part, filtered_colon_position + 1);
                            filter_number = atof(number_part);
                        } else {
                            printf("ERROR: missing second colon in parameters\n");
                        }
                    } else {
                        printf("ERROR: missing colon in parameters\n");
                    }

                    // DO FILTERING
                    Demographics *filtered = filter_numeric(&record_count, counties, filter_field, filter_ge_le, filter_number);

                    if (filtered != NULL) {
                        free(counties);
                        counties = filtered;
                        free(filtered);
                    }

                    printf("Filter: %s %s %f (%d entries)\n", filter_field, filter_ge_le, filter_number, record_count);
                } else {
                    fprintf(stderr, "ERROR: could not find command\n");
                    return 1;
                }
            } else {
                fprintf(stderr, "ERROR: invalid command %s\n", line);
                return 1;
            }
        }        
    }
    fclose(ops);

    return 0;
 }

Demographics* filter_numeric(int *record_count, Demographics counties[], char* field, char *operation, float number) {
    int new_record_count = 0;
    Demographics *filtered_counties = malloc(*record_count * sizeof(Demographics));
    if (filtered_counties == NULL) {
        perror("could not allocate memory");
        *record_count = 0;
        return NULL;
    }

    if (strcmp(field, "Education.High School or Higher") == 0) {
        if (strcmp(operation, "ge") == 0) {
            // GREATER THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].education_high_school_or_higher >= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
        } else if (strcmp(operation, "le") == 0) {
            // LESS THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].education_high_school_or_higher <= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
            
        }
    } else if (strcmp(field, "Education.Bachelor's Degree or Higher") == 0) {
        if (strcmp(operation, "ge") == 0) {
            // GREATER THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].education_bachelors_or_higher >= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
        } else if (strcmp(operation, "le") == 0) {
            // LESS THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].education_bachelors_or_higher <= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
            
        }
    } else if (strcmp(field, "Ethnicities.American Indian and Alaska Native Alone") == 0) {
        if (strcmp(operation, "ge") == 0) {
            // GREATER THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].ethnicities_american_indian_alaska_native >= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
        } else if (strcmp(operation, "le") == 0) {
            // LESS THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].ethnicities_american_indian_alaska_native <= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
            
        }
    } else if (strcmp(field, "Ethnicities.Asian Alone") == 0) {
        if (strcmp(operation, "ge") == 0) {
            // GREATER THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].ethnicities_asian >= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
        } else if (strcmp(operation, "le") == 0) {
            // LESS THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].ethnicities_asian <= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
            
        }
    } else if (strcmp(field, "Ethnicities.Black Alone") == 0) {
        if (strcmp(operation, "ge") == 0) {
            // GREATER THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].ethnicities_black >= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
        } else if (strcmp(operation, "le") == 0) {
            // LESS THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].ethnicities_black <= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
            
        }
    } else if (strcmp(field, "Ethnicities.Hispanic or Latino") == 0) {
        if (strcmp(operation, "ge") == 0) {
            // GREATER THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].ethnicities_hispanic_or_latino >= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
        } else if (strcmp(operation, "le") == 0) {
            // LESS THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].ethnicities_hispanic_or_latino <= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
            
        }
    } else if (strcmp(field, "Ethnicities.Native Hawaiian and Other Pacific Islander Alone") == 0) {
        if (strcmp(operation, "ge") == 0) {
            // GREATER THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].ethnicities_hawaiian >= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
        } else if (strcmp(operation, "le") == 0) {
            // LESS THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].ethnicities_hawaiian <= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
            
        }
    } else if (strcmp(field, "Ethnicities.Two or More Races") == 0) {
        if (strcmp(operation, "ge") == 0) {
            // GREATER THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].ethnicities_two_or_more >= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
        } else if (strcmp(operation, "le") == 0) {
            // LESS THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].ethnicities_two_or_more <= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
            
        }
    } else if (strcmp(field, "Ethnicities.White Alone") == 0) {
        if (strcmp(operation, "ge") == 0) {
            // GREATER THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].ethnicities_white >= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
        } else if (strcmp(operation, "le") == 0) {
            // LESS THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].ethnicities_white <= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
            
        }
    } else if (strcmp(field, "Ethnicities.White Alone not Hispanic or Latino") == 0) {
        if (strcmp(operation, "ge") == 0) {
            // GREATER THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].ethnicities_white_not_hispanic >= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
        } else if (strcmp(operation, "le") == 0) {
            // LESS THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].ethnicities_white_not_hispanic <= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
            
        }
    } else if (strcmp(field, "Income.Median Household Income") == 0) {
        if (strcmp(operation, "ge") == 0) {
            // GREATER THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].income_median_household >= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
        } else if (strcmp(operation, "le") == 0) {
            // LESS THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].income_median_household <= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
            
        }
    } else if (strcmp(field, "Income.Per Capita Income") == 0) {
        if (strcmp(operation, "ge") == 0) {
            // GREATER THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].income_per_capita >= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
        } else if (strcmp(operation, "le") == 0) {
            // LESS THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].income_per_capita <= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
            
        }
    } else if (strcmp(field, "Income.Persons Below Poverty Level") == 0) {
        if (strcmp(operation, "ge") == 0) {
            // GREATER THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].income_persons_below_poverty >= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
        } else if (strcmp(operation, "le") == 0) {
            // LESS THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].income_persons_below_poverty <= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
            
        }
    } else if (strcmp(field, "Population.2014 Population") == 0) {
        if (strcmp(operation, "ge") == 0) {
            // GREATER THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].population_2014 >= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
        } else if (strcmp(operation, "le") == 0) {
            // LESS THAN OR EQUAL TO
            for (int i = 0; i < *record_count; i++) {
                if (counties[i].population_2014 <= number) {
                    filtered_counties[new_record_count++] = counties[i];
                }
            }
            
        }
    }

    *record_count = new_record_count;
    return filtered_counties;
}

Demographics* filter_state(int *record_count, Demographics counties[], char* state) {
    int new_record_count = 0;
    Demographics *filtered_counties = malloc(*record_count * sizeof(Demographics));
    if (filtered_counties == NULL) {
        perror("could not allocate memory");
        *record_count = 0;
        return NULL;
    }

    for (int i = 0; i < *record_count; i++) {
        if (strcmp(counties[i].state, state) == 0) {
            filtered_counties[new_record_count++] = counties[i];
        }
    }

    *record_count = new_record_count;
    return filtered_counties;
}

double population(int record_count, Demographics counties[], char* type) {
    // RETURNS TOTAL POPULATION COUNT OF TYPE
    double total = 0.0;

    if (strcmp(type, "total") == 0) {
        for (int i = 0; i < record_count; i++) {
            //printf("%d\n", counties[i].population_2014);
            total += counties[i].population_2014;
        }
    }
    // ELSE IF FOR OTHER APPLICABLE POPULATION CALCULATORS
    // FORMULA: (.01*percentage)(population)
    else if (strcmp(type, "Education.Bachelor's Degree or Higher") == 0) {
        for (int i = 0; i < record_count; i++) {
            total += counties[i].education_bachelors_or_higher * counties[i].population_2014;
        }

        total = round(total * 0.01);
    } else if (strcmp(type, "Education.High School or Higher") == 0) {
        for (int i = 0; i < record_count; i++) {
            total += counties[i].education_high_school_or_higher * counties[i].population_2014;
        }

        total = round(total * 0.01);
    } else if (strcmp(type, "Ethnicities.American Indian and Alaska Native Alone") == 0) {
        for (int i = 0; i < record_count; i++) {
            total += counties[i].ethnicities_american_indian_alaska_native * counties[i].population_2014;
        }

        total = round(total * 0.01);
    } else if (strcmp(type, "Ethnicities.Asian Alone") == 0) {
        for (int i = 0; i < record_count; i++) {
            total += counties[i].ethnicities_asian * counties[i].population_2014;
        }

        total = round(total * 0.01);
    } else if (strcmp(type, "Ethnicities.Black Alone") == 0) {
        for (int i = 0; i < record_count; i++) {
            total += counties[i].ethnicities_black * counties[i].population_2014;
        }

        total = round(total * 0.01);
    } else if (strcmp(type, "Ethnicities.Hispanic or Latino") == 0) {
        for (int i = 0; i < record_count; i++) {
            total += counties[i].ethnicities_hispanic_or_latino * counties[i].population_2014;
        }

        total = round(total * 0.01);
    } else if (strcmp(type, "Ethnicities.Native Hawaiian and Other Pacific Islander Alone") == 0) {
        for (int i = 0; i < record_count; i++) {
            total += counties[i].ethnicities_hawaiian * counties[i].population_2014;
        }

        total = round(total * 0.01);
    } else if (strcmp(type, "Ethnicities.Two or More Races") == 0) {
        for (int i = 0; i < record_count; i++) {
            total += counties[i].ethnicities_two_or_more * counties[i].population_2014;
        }

        total = round(total * 0.01);
    } else if (strcmp(type, "Ethnicities.White Alone") == 0) {
        for (int i = 0; i < record_count; i++) {
            total += counties[i].ethnicities_white * counties[i].population_2014;
        }

        total = round(total * 0.01);
    } else if (strcmp(type, "Ethnicities.White Alone, not Hispanic or Latino") == 0) {
        for (int i = 0; i < record_count; i++) {
            total += counties[i].ethnicities_white_not_hispanic * counties[i].population_2014;
        }

        total = round(total * 0.01);
    } else if (strcmp(type, "Income.Persons Below Poverty Level") == 0) {
        for (int i = 0; i < record_count; i++) {
            total += counties[i].income_persons_below_poverty * counties[i].population_2014;
        }

        total = round(total * 0.01);
    }

    return total;
}

void display(int record_count, Demographics counties[]) {
    for (int i = 0; i < record_count; i++) {
        printf("%s, %s\n", counties[i].county, counties[i].state);
        printf("    Population: %d\n", counties[i].population_2014);
        printf("    Education\n");
        printf("        >= High School: %.6f%%\n", counties[i].education_high_school_or_higher);
        printf("        >= Bachelor's: %.6f%%\n", counties[i].education_bachelors_or_higher);
        printf("    Ethnicity Percentages\n");
        printf("        American Indian and Alaska Native: %.6f%%\n", counties[i].ethnicities_american_indian_alaska_native);
        printf("        Asian Alone: %.6f%%\n", counties[i].ethnicities_asian);
        printf("        Black Alone: %.6f%%\n", counties[i].ethnicities_black);
        printf("        Hispanic or Latino: %.6f%%\n", counties[i].ethnicities_hispanic_or_latino);
        printf("        White Alone: %.6f%%\n", counties[i].ethnicities_white);
        printf("        White Alone, not Hispanic or Latino: %.6f%%\n", counties[i].ethnicities_white_not_hispanic);
        printf("    Income\n");
        printf("        Median Household: %d\n", counties[i].income_median_household);
        printf("        Per Capita: %d\n", counties[i].income_per_capita);
        printf("        Below Poverty Level: %.6f%%\n", counties[i].income_persons_below_poverty);
    }
}
 