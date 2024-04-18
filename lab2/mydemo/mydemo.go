package main

import "fmt"

func main() {
	a := 32
	b := 2
	b *= 2;
	fmt.Println(a * b, a << 4, b * 32 * 3 / 4, 4 * a, 4 * 32)
}
