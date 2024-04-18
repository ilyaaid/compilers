package main

import "fmt"

func main() {
	a := 32
	b := 2
	b *= 2
	fmt.Println(a*b, a<<4, b<<5*3>>2, a<<2, 4<<5)
}
