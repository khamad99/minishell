/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalshaer <kalshaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 08:15:35 by kalshaer          #+#    #+#             */
/*   Updated: 2023/05/17 10:09:54 by kalshaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void sort_words(char **words, int num_words) {
    int i, j, k;
    char *temp;

    // Sort the char* strings using bubble sort
    for (i = 0; i < num_words - 1; i++) {
        for (j = 0; j < num_words - i - 1; j++) {
            // Compare the first letter of the current and next word
            if (words[j][0] > words[j + 1][0]) {
                // Swap the words if they are out of order
                temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            } else if (words[j][0] == words[j + 1][0]) {
                // If the first letter is the same, compare subsequent letters
                k = 1;
                while (words[j][k] != '\0' && words[j + 1][k] != '\0') {
                    if (words[j][k] > words[j + 1][k]) {
                        // Swap the words if they are out of order
                        temp = words[j];
                        words[j] = words[j + 1];
                        words[j + 1] = temp;
                        break;
                    } else if (words[j][k] < words[j + 1][k]) {
                        // Stop comparing if the current letter is smaller
                        break;
                    }
                    k++;
                }
                // If both words are equal so far, sort based on word length
                if (words[j][k] == '\0' && words[j + 1][k] == '\0' && strlen(words[j]) > strlen(words[j + 1])) {
                    temp = words[j];
                    words[j] = words[j + 1];
                    words[j + 1] = temp;
                }
            }
        }
    }
}


