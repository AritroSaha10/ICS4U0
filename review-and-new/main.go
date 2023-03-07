/**
 * Description: EthicalCompute, a quizzing program that helps educate people about ethical computing.
 * Author: Aritro Saha
 * Last edited: Feb. 22, 2023
 */
package main

import (
	"encoding/json"
	"errors"
	"fmt"
	"io/ioutil"
	"net/http"
	"strconv"
	"time"
)

type Typing_Speed float64
type Delay_Setting float64

const (
	Slow_Typing   Typing_Speed = 0.1 // factor on seconds
	Medium_Typing              = 0.05
	Fast_Typing                = 0.01
)

const (
	No_Delay     Delay_Setting = 0 // seconds
	Short_Delay                = 0.5
	Medium_Delay               = 1
	Long_Delay                 = 2
)

// A set of questions from a loaded in JSON file
type QuestionSet struct {
	Questions []QuestionAnswer `json:"questions"`
}

// A question and answer from the loaded JSON file
type QuestionAnswer struct {
	Question string         `json:"question"`
	Options  []AnswerOption `json:"options"`
}

// An option for a question from the loaded JSON file
type AnswerOption struct {
	Answer      string `json:"answer"`
	IsCorrect   bool   `json:"correct"`
	Explanation string `json:"explanation"`
}

// Global variables
var typingFactor float64 = 2
var questionCount int64 = 10
var score int = 0
var questionSet QuestionSet

// Slowly prints out a string provided
func slowPrint(str string, delay Typing_Speed, delayEnd Delay_Setting) {
	for _, char := range str {
		fmt.Printf("%s", string(char))
		time.Sleep(time.Duration(float64(delay) / typingFactor * float64(time.Second)))
	}
	time.Sleep(time.Duration(float64(delayEnd) / typingFactor * float64(time.Second)))
}

// Prompts the user to press the enter key, while also cleaning up said prompt
func waitForUserPrompt() {
	slowPrint("Press enter to continue...", 0.01, 0)
	var tmp string
	fmt.Scanf("%s\n", &tmp)
	fmt.Print("\033[1A\033[K") // Terminal character that clears current line
}

// Calculate a random number between 0 and the maximum number, for getting random elements from array
func GetPseudoRandomIdx(max int) int {
	return int((time.Now().Unix()/7 + 8392) % int64(max))
}

// Returns the correct answer out of a slice of AnswerOptions, as well as its index, through a linear search.
// Also provides an error if it could not find one.
func FindCorrectAnswer(options []AnswerOption) (int, AnswerOption, error) {
	for i, option := range options {
		if option.IsCorrect {
			return i, option, nil
		}
	}
	return -1, AnswerOption{}, errors.New("Could not find correct answer")
}

// Removes an element at index s from a QuestionAnswer slice.
func remove(slice []QuestionAnswer, s int) []QuestionAnswer {
	return append(slice[:s], slice[s+1:]...)
}

// Returns a string representation of a time
func formatTime(seconds int) string {
	hours := seconds / 3600                // Integer division to get hours
	minutes := (seconds - 3600*hours) / 60 // Integer division to get minutes
	seconds = seconds % 60                 // Modulo to get remaining seconds

	if hours != 0 {
		return fmt.Sprintf("%02d:%02d:%02ds", hours, minutes, seconds)
	} else {
		return fmt.Sprintf("%02d:%02ds", minutes, seconds)
	}
}

// Returns a string representation of a time delta between a start and end time (in seconds)
func formatTimeDelta(startTime int, endTime int) string {
	return formatTime(endTime - startTime)
}

func main() {
	// Retrieve the question set data from an online source
	reqRes, err := http.Get("https://gist.githubusercontent.com/AritroSaha10/982dcd63ccd0b23ac4c0100951f3b9cb/raw/3e278cff9565b4f4f043d2d846485defc0eb10ad/question-set.json")
	if err != nil {
		panic(err)
	}
	datasetRaw, err := ioutil.ReadAll(reqRes.Body)
	if err != nil {
		panic(err)
	}

	// Convert the received data from online source into serialized data
	json.Unmarshal(datasetRaw, &questionSet)

	// Title screen
	fmt.Println(`
 
	 ______ _   _     _           _  _____                            _       
	 |  ____| | | |   (_)         | |/ ____|                          | |      
	 | |__  | |_| |__  _  ___ __ _| | |     ___  _ __ ___  _ __  _   _| |_ ___ 
	 |  __| | __| '_ \| |/ __/ _' | | |    / _ \| '_ ' _ \| '_ \| | | | __/ _ \
	 | |____| |_| | | | | (_| (_| | | |___| (_) | | | | | | |_) | |_| | ||  __/
	 |______|\__|_| |_|_|\___\__,_|_|\_____\___/|_| |_| |_| .__/ \__,_|\__\___|
				                              | |                  
				                              |_|                  
	 
 `)
	slowPrint("Welcome to EthicalCompute! ", Medium_Typing, Medium_Delay)
	slowPrint("EthicalCompute is a program designed to educate everyone about ethical computing practices.\n", Medium_Typing, Short_Delay)
	waitForUserPrompt()
	slowPrint("This program assesses the user on their knowledge of Computer Ethics, providing detailed explanations for questions.\n", Medium_Typing, Medium_Delay)
	slowPrint("Before we start, let's get some important info.\n", Medium_Typing, Medium_Delay)

	// Ask user for their preferred typing speed
	// Unfortunately, this can't be turned into a function because it wouldn't be able to handle converting to different types
	for {
		var tmp string
		slowPrint("1. What is your preferred typing speed factor? Choose a number between 0.1 and 10: ", Medium_Typing, No_Delay)
		fmt.Scanf("%s\n", &tmp)

		tmpFactor, err := strconv.ParseFloat(tmp, 64)
		if err == nil && tmpFactor >= 0.1 && tmpFactor <= 10 {
			slowPrint(fmt.Sprintf("Current typing speed: %.2f\n", tmpFactor), Medium_Typing, Medium_Delay)
			typingFactor = tmpFactor
			break
		} else {
			fmt.Println("Please provide valid input.")
			waitForUserPrompt()
			fmt.Print("\033[1A\033[K\033[1A\033[K")
		}
	}

	waitForUserPrompt()

	// Ask user for their preferred # of questions. Dynamically changes the maximum to however many questions exist
	// Unfortunately, this can't be turned into a function because it wouldn't be able to handle converting to different types
	for {
		var tmp string
		slowPrint(fmt.Sprintf("2. How many questions do you want to do? Choose a number between 1 and %d: ", len(questionSet.Questions)), Medium_Typing, No_Delay)
		fmt.Scanf("%s\n", &tmp)

		tmpQCount, err := strconv.ParseInt(tmp, 10, 64)
		if err == nil && tmpQCount >= 1 && tmpQCount <= int64(len(questionSet.Questions)) {
			slowPrint("Great! Let's get started. Just press enter to start the questions.\n", Medium_Typing, Medium_Delay)
			questionCount = tmpQCount
			break
		} else {
			fmt.Println("Please provide valid input.")
			waitForUserPrompt()
			fmt.Print("\033[1A\033[K\033[1A\033[K")
		}
	}

	waitForUserPrompt()

	// Record start time to calculate time delta later
	startTime := time.Now().Unix()

	// Iterate through the number of questions user asked for
	for i := 1; i <= int(questionCount); i++ {
		// Pick out a random question
		chosenQuestionIdx := GetPseudoRandomIdx(len(questionSet.Questions))
		chosenQuestion := questionSet.Questions[chosenQuestionIdx]

		// Display the question statement to the user.
		questionStatement := fmt.Sprintf("Q%d: %s\n", i, chosenQuestion.Question)
		slowPrint(questionStatement, Medium_Typing, Short_Delay)

		// Display the possible options to the user.
		for optI := 0; optI < len(chosenQuestion.Options); optI++ {
			option := chosenQuestion.Options[optI]
			optionStatement := fmt.Sprintf("  %d: %s\n", optI+1, option.Answer)

			slowPrint(optionStatement, Medium_Typing, Short_Delay)
		}

		fmt.Println()

		// Retrieve the user's answer as an integer.
		var userAns int // 0 to index of last option
		for {
			var tmp string
			slowPrint("What would you do (number only): ", Medium_Typing, No_Delay)
			fmt.Scanf("%s\n", &tmp)

			tmpAns, err := strconv.ParseInt(tmp, 10, 64)
			if err == nil && tmpAns >= 1 && tmpAns <= int64(len(chosenQuestion.Options)) {
				userAns = int(tmpAns)
				break
			} else {
				fmt.Println("Please provide valid input.")
				waitForUserPrompt()
				fmt.Print("\033[1A\033[K\033[1A\033[K")
			}
		}

		fmt.Println()

		// Handle whether the user's answer is correct or not
		if chosenQuestion.Options[userAns-1].IsCorrect {
			correctAnsTxt := fmt.Sprintf("Congrats, you're correct! Here's the explanation for the answer:\n%s\n", chosenQuestion.Options[userAns-1].Explanation)
			slowPrint(correctAnsTxt, Medium_Typing, Medium_Delay)
			score += 1
		} else {
			goodAnsIdx, _, err := FindCorrectAnswer(chosenQuestion.Options)
			if err != nil {
				panic(err)
			}

			incorrectAnsTxt := fmt.Sprintf("Aw, you were wrong :( The correct answer was %d. Here's the explanation:\n%s\n", goodAnsIdx+1, chosenQuestion.Options[userAns-1].Explanation)
			slowPrint(incorrectAnsTxt, Medium_Typing, Medium_Delay)
		}

		// Display the user's current score on the screen.
		slowPrint(fmt.Sprintf("\nYour current score is %d.\n\n", score), Medium_Typing, Long_Delay)

		// Remove the question from our dataset so we don't ask it again.
		questionSet.Questions = remove(questionSet.Questions, chosenQuestionIdx)
	}

	// Display user's final score and percentage.
	typingFactor = 2
	slowPrint(fmt.Sprintf("Your final score is........\n"), Slow_Typing, Long_Delay)
	slowPrint(fmt.Sprintf("%d! That's a %.1f%%.\n", score, float32(score)/float32(questionCount)*100), Medium_Typing, Long_Delay)

	// Calculate time delta, and get average time to complete 1 question
	endTime := time.Now().Unix()
	timeDeltaStr := formatTimeDelta(int(startTime), int(endTime))
	avgTime := int(endTime-startTime) / int(questionCount)
	avgTimeStr := formatTime(avgTime)
	slowPrint(fmt.Sprintf("Time to complete all questions: %s\n", timeDeltaStr), Medium_Typing, Short_Delay)
	slowPrint(fmt.Sprintf("Average time to complete each question: %s\n", avgTimeStr), Medium_Typing, Long_Delay)

	fmt.Println()
	slowPrint("Thanks for taking the quiz! If you'd like to try again, feel free to run the program another time. Goodbye!\n", Medium_Typing, Medium_Delay)
}
