package main

import (
	"github.com/astaxie/beego"
	_ "quickstart/models"
	_ "quickstart/routers"
)

func main() {
	beego.Run("localhost:8080")
}

