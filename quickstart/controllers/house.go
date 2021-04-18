package controllers

import (
	"fmt"
	"github.com/astaxie/beego"
	"github.com/astaxie/beego/orm"
	"quickstart/models"
)

type HouseController struct {
	beego.Controller
}

func (c *HouseController) HouseGet() {
	o := orm.NewOrm()
	qs := o.QueryTable("house")
	houseData := []models.House{}
	qs.All(&houseData)

	frontData := make([][]int, len(houseData))
	for i := range frontData {
		frontData[i] = make([]int, 7)
	}

	for i, v := range houseData {
		frontData[i][0] = v.Id
		frontData[i][1] = v.Space
		frontData[i][2] = v.Price
		frontData[i][3] = v.Direction
		frontData[i][4] = v.Floor
		frontData[i][5] = v.Age
		frontData[i][6] = v.Emergency
	}

	fmt.Println(frontData)
	c.Data["arr"] = frontData
	c.TplName = "houseInfo.html"
}

func (c *HouseController) HousePost() {

	c.TplName = "detail.html"
}

