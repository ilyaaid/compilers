package main

import (
	"fmt"
	"go/ast"
	"go/format"
	"go/parser"
	"math"
	"strconv"

	//"go/printer"
	"go/token"
	"os"
)

func insertBinShift(file *ast.File) {
	// Вызываем обход дерева, начиная от корня
	ast.Inspect(file, func(node ast.Node) bool {
		if binaryExpr, ok := node.(*ast.BinaryExpr); ok {
			y, ok := binaryExpr.Y.(*ast.BasicLit)

			changed := false
			if ok && y.Kind.String() == "INT" {
				i, err := strconv.Atoi(y.Value)
				if err != nil {
					panic(err)
				}
				st := math.Log2(float64(i))
				if math.Pow(2, st) == float64(i) {
					if binaryExpr.Op == token.MUL {
						binaryExpr.Op = token.SHL
						y.Value = strconv.Itoa(int(st))
						changed = true
					} else if binaryExpr.Op == token.QUO {
						binaryExpr.Op = token.SHR
						y.Value = strconv.Itoa(int(st))
						changed = true
					}
				}
			}

			if !changed {
				x, ok := binaryExpr.X.(*ast.BasicLit)
				if ok && x.Kind.String() == "INT" {
					i, err := strconv.Atoi(x.Value)
					if err != nil {
						panic(err)
					}
					st := math.Log2(float64(i))

					binaryExpr.X, binaryExpr.Y = binaryExpr.Y, binaryExpr.X
					y, _ := binaryExpr.Y.(*ast.BasicLit)
					if math.Pow(2, st) == float64(i) {
						if binaryExpr.Op == token.MUL {
							binaryExpr.Op = token.SHL
							y.Value = strconv.Itoa(int(st))
						}
					}
				}
			}
		}
		return true
	})
}

/*
Нужно заменить (при делении или умножении на число 2^x)
все знаки деления '/' на '>>' (побитовый сдвиг вправо)
все знаки умножения '*' на '<<' (побитовый сдвиг влево)
*/

func main() {
	if len(os.Args) != 2 {
		return
	}

	fset := token.NewFileSet()
	if file, err := parser.ParseFile(fset, os.Args[1], nil, parser.ParseComments); err == nil {
		insertBinShift(file)

		if format.Node(os.Stdout, fset, file) != nil {
			fmt.Printf("Formatter error: %v\n", err)
		}
		//ast.Fprint(os.Stdout, fset, file, nil)
	} else {
		fmt.Printf("Errors in %s\n", os.Args[1])
	}
}
