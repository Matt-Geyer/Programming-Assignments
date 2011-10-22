Matt Geyer
CS 326
FizzBuzz
Haskell #1

This function takes an integer as a parameter and returns appropriate
string corresponding to that integer.

>buildFizzBuzzEntry :: Integer -> String
>buildFizzBuzzEntry x
>    | x `mod` 3 == 0 &&  x `mod` 5 == 0  = ( show x ++ " Fizz Buzz" )
>    | x `mod` 3 == 0 = ( show x ++ " Fizz" )
>    | x `mod` 5 == 0 = ( show x ++ " Buzz" )
>    | otherwise = show x 

This function accepts a list of integers and returns a list
of strings corresponding to each integer as defined by the 
fizz buzz problem.

>fizzBuzz :: [Integer] -> [String]
>fizzBuzz l = map buildFizzBuzzEntry l 

>main = do
>    -- Get user input and convert from a String to Integer
>    -- so that it can be used to create a range
>    putStrLn "Please enter the ending number in the range to check: "
>    input <- getLine
>    let range = read input :: Integer
>    -- Call the fizzBuzz function with the range as an argument
>    let fizzList = fizzBuzz [1..range]
>    -- Use mapM_ to sequence the list and print every string        
>    mapM_ putStrLn fizzList
