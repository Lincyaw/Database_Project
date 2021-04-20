package controllers

import (
	"encoding/json"
	"fmt"
	"github.com/astaxie/beego"
	"github.com/astaxie/beego/orm"
	"quickstart/models"
	"strconv"
)

type MainController struct {
	beego.Controller
}

func (c *MainController) Get() {
	// 增
	//// 1. 有orm对象
	//o := orm.NewOrm()
	//// 2. 有要插入数据的结构体对象
	//user := models.User{}
	//// 3. 对结构体对象赋值
	//user.Name = "111"
	//user.Pwd = "222"
	//// 4. 插入
	//_, err :=o.Insert(&user)
	//if err != nil{
	//	beego.Info(err)
	//}

	// 查
	// 1. 有orm对象
	//o := orm.NewOrm()
	//// 2. 查询的对象
	//user := models.User{}
	//// 3. 指定查询对象的字段值
	//user.Name = "111"
	//// 4. 查询
	//err := o.Read(&user,"Name")
	//
	//if err != nil{
	//	beego.Info(err)
	//}
	//beego.Info("查询成功", user)

	// 改
	// 1. 有orm对象
	//o := orm.NewOrm()
	////2. 需要更新的结构体对象
	//user := models.User{}
	////3. 查到需要更新的数据
	//user.Id = 1
	//err := o.Read(&user)
	////4. 给数据重新赋值
	//if err == nil {
	//	user.Name = "222"
	//	user.Pwd = "3334"
	//	//5. 更新
	//	_,err := o.Update(&user)
	//	if err != nil{
	//		beego.Info(err)
	//		return
	//	}
	//	beego.Info("更新成功")
	//}

	c.Redirect("/login", 302)
}
func (c *MainController) ShowRegister() {
	c.TplName = "register.html"
}
func (c *MainController) HandleRegister() {
	// 拿到数据
	iszhongjie := c.GetString("zhongjie")
	userName := c.GetString("userName")
	phone := c.GetString("phoneNumber")
	pwd := c.GetString("pwd")
	pwd2 := c.GetString("pwd2")
	beego.Info(userName, pwd)
	// 对数据进行校验
	if userName == "" || pwd == "" || phone == "" || pwd != pwd2 {
		beego.Info("数据不能为空")
		c.Redirect("/register", 302)
	}
	if len(phone) != 11 {
		c.Redirect("/register", 302)
	}
	// 插入数据库
	o := orm.NewOrm()
	if iszhongjie != "on" {
		user := models.User{}
		user.Name = userName
		user.Pwd = pwd
		user.PhoneNum = phone
		user.Wechat = c.GetString("wechat")
		user.Mail = c.GetString("mail")
		_, err := o.Insert(&user)
		if err != nil {
			beego.Info("插入数据失败", err)
			c.Redirect("/register", 302)
			return
		}
	} else {
		user := models.Inter{}
		user.Name = userName
		user.Pwd = pwd
		user.PhoneNum = phone
		user.Wechat = c.GetString("wechat")
		user.Mail = c.GetString("mail")
		_, err := o.Insert(&user)
		if err != nil {
			beego.Info("插入数据失败", err)
			c.Redirect("/register", 302)
			return
		}
	}

	// 返回登录界面
	c.Redirect("/login", 302)

	//c.Ctx.WriteString("注册成功")
}
func (c *MainController) ShowLogin() {
	c.TplName = "login.html"
}

func (c *MainController) HandleLogin() {
	// 1. 拿到数据
	userName := c.GetString("userName")
	iszhongjie := c.GetString("zhongjie")
	pwd := c.GetString("pwd")
	// 2. 判断数据是否合法
	if userName == "" || pwd == "" {
		beego.Info("输入数据不合法")
		c.TplName = "login.html"
		return
	}
	// 3. 查询账号密码是否正确
	o := orm.NewOrm()
	if iszhongjie != "on" {
		user := models.User{}
		user.Name = userName
		err := o.Read(&user, "Name")
		if err != nil {
			beego.Info("查询失败")
			c.TplName = "login.html"
			return
		}
		if user.Pwd != pwd {
			beego.Info("密码错误")
			c.TplName = "login.html"
			return
		}
		//c.SetSession("userName", userName)
		c.SetSession("userId", user.Id)
		c.SetSession("isInter", false)
		// 4. 跳转
		c.Redirect("/houseInfo", 302)
	} else {
		user := models.Inter{}
		user.Name = userName
		err := o.Read(&user, "Name")
		if err != nil {
			beego.Info("查询失败")
			c.TplName = "login.html"
			return
		}
		if user.Pwd != pwd {
			beego.Info("密码错误")
			c.TplName = "login.html"
			return
		}
		//c.SetSession("userName", userName)
		c.SetSession("userId", user.Id)
		c.SetSession("isInter", true)
		// 4. 跳转
		c.Redirect("/houseInfo", 302)
	}

}

func (c *MainController) UserInfoGet() {
	userid := c.GetSession("userId")
	if userid == nil {
		c.Redirect("/login", 302)
		return
	}
	isInter := c.GetSession("isInter")
	o := orm.NewOrm()
	// 不是中介
	if isInter == false {
		qs := o.QueryTable("User")
		user := models.User{}
		err := qs.Filter("id__exact", userid).One(&user)
		if err != nil {
			fmt.Println(err)
			return
		}
		fmt.Println(user)
		c.Data["Id"] = user.Id
		fmt.Println(c.Data["Id"])
		c.Data["Mail"] = user.Mail
		c.Data["Wechat"] = user.Wechat
		c.Data["PhoneNum"] = user.PhoneNum
		c.Data["Name"] = user.Name
		/* -------------------------------*/
		// todo: 显示收藏
		m2m := o.QueryM2M(&user,"Favourites")
		num,err:=m2m.Count()
		//o.Read(&user)
		//fmt.Println(user)
		/* -------------------------------*/

		fmt.Println("收藏 ", user.Favourites,num)
		favs := make([]int, len(user.Favourites))
		for i, fav := range user.Favourites {
			favs[i] = fav.Id
		}
		c.Data["Favs"] = favs

		appointment := []models.Appointment{}
		qs = o.QueryTable("Appointment")
		_, err = qs.Filter("user_id", userid).All(&appointment)
		fmt.Println("user info appointment", err)
		houseId := make([]int, len(appointment))
		for i, app := range appointment {
			houseId[i] = app.House.Id
		}
		c.Data["Appointments"] = houseId

	} else {
		// 是中介
		qs := o.QueryTable("Inter")
		data := models.Inter{}
		err := qs.Filter("id__exact", userid).One(&data)
		if err != nil {
			fmt.Println(err)
			return
		}
		fmt.Println(data)
		c.Data["Id"] = data.Id
		fmt.Println(c.Data["Id"])
		c.Data["Mail"] = data.Mail
		c.Data["Wechat"] = data.Wechat
		c.Data["PhoneNum"] = data.PhoneNum
		c.Data["Name"] = data.Name
	}
	c.TplName = "userInfo.html"
}
func (c *MainController) UserInfoPost() {
	// 并且能够删除
	userName := c.GetString("userName")
	if userName == "" {
		c.TplName = "changeInfo.html"
		return
	}
	// 拿到数据
	phone := c.GetString("phoneNumber")
	pwd := c.GetString("pwd")
	pwd2 := c.GetString("pwd2")
	beego.Info(userName, pwd)
	// 对数据进行校验
	if userName == "" || pwd == "" || phone == "" || pwd != pwd2 {
		beego.Info("数据不能为空")
		c.Redirect("/user", 302)
	}
	if len(phone) != 11 {
		c.Redirect("/c", 302)
	}
	// 更新数据库
	userid := c.GetSession("userId")
	o := orm.NewOrm()
	qs := o.QueryTable("user")
	num, err := qs.Filter("id", userid).Update(orm.Params{
		"name":     userName,
		"pwd":      pwd,
		"phoneNum": phone,
		"wechat":   c.GetString("wechat"),
		"mail":     c.GetString("mail"),
	})
	beego.Info("成功执行了", num, "条")
	if err != nil {
		beego.Info("更新数据失败", err)
		c.Redirect("/user", 302)
		return
	}

	// 返回登录界面
	c.Redirect("/login", 302)

}
func (c *MainController) AboutGet() {
	c.TplName = "about.html"
}
func (c *MainController) ExitGet() {
	c.DelSession("userId")
	c.Redirect("/login", 302)
}

func (c *MainController) SendGet() {
	userid := c.GetSession("userId")
	if userid == nil {
		c.Redirect("/login", 302)
		return
	}

	c.TplName = "addHouse.html"
}

func (c *MainController) SendPost() {
	userid := c.GetSession("userId")
	if userid == nil {
		c.Redirect("/login", 302)
		return
	}
	space, err := c.GetInt("space")
	if err != nil {
		fmt.Println(err)
	}
	price, err := c.GetInt("price")
	if err != nil {
		fmt.Println(err)
	}
	dir, err := c.GetInt("dir")
	if err != nil {
		fmt.Println(err)
	}
	floor, err := c.GetInt("floor")
	if err != nil {
		fmt.Println(err)
	}
	rooms, err := c.GetInt("rooms")
	if err != nil {
		fmt.Println(err)
	}
	age, err := c.GetInt("age")
	if err != nil {
		fmt.Println(err)
	}
	emergency, err := c.GetInt("emergency")
	if err != nil {
		fmt.Println(err)
	}
	station := c.GetString("station")
	line := c.GetString("line")
	area := c.GetString("area")
	property := c.GetString("property")
	Htype := c.GetString("type")
	decoration := c.GetString("decoration")
	fmt.Println(station, line, area, property, Htype, decoration)
	houseInfo := models.House{}

	o := orm.NewOrm()
	qs := o.QueryTable("Inter")

	userdata := models.Inter{}
	err = qs.Filter("id__exact", userid).One(&userdata)
	if err != nil {
		fmt.Println(err)
		c.Redirect("/login", 302)
		return
	}
	houseInfo.Inter = &userdata
	houseInfo.Space = space
	houseInfo.Price = price
	houseInfo.Direction = dir
	houseInfo.Floor = floor
	houseInfo.Age = age
	houseInfo.Emergency = emergency
	o.Begin()
	/*--------------------------------------------------------------------*/
	qs = o.QueryTable("rooms")
	tmpRooms := models.Rooms{}
	err = qs.Filter("rooms", rooms).One(&tmpRooms)
	// 如果没有这样的房间，就往表里插入
	if err != nil {
		tmpRooms.Rooms = rooms
		//tmpRooms.Houses = append(tmpRooms.Houses, &houseInfo)
		_, err = o.Insert(&tmpRooms)
		if err != nil {
			fmt.Println("插入失败", err)
			o.Rollback()
		}
	}
	// 如果有的话，就把这个对象加到house里
	houseInfo.Rooms = &tmpRooms
	/*--------------------------------------------------------------------*/
	qs = o.QueryTable("subway")
	tmpSub := models.Subway{}
	err = qs.Filter("station", station).One(&tmpSub)
	// 如果没有这样的房间，就往表里插入
	if err != nil {
		tmpSub.Station = station
		tmpSub.Line = line
		_, err = o.Insert(&tmpSub)
		if err != nil {
			fmt.Println("插入失败", err)
			o.Rollback()
		}
	}
	// 如果有的话，就把这个对象加到house里
	houseInfo.Subway = &tmpSub
	tmpSub.Houses = append(tmpSub.Houses, &houseInfo)
	/*--------------------------------------------------------------------*/
	qs = o.QueryTable("area")
	tmparea := models.Area{}
	err = qs.Filter("area", area).One(&tmparea)
	// 如果没有这样的房间，就往表里插入
	if err != nil {
		tmparea.Area = area
		_, err = o.Insert(&tmparea)
		if err != nil {
			fmt.Println("插入失败", err)
			o.Rollback()
		}
	}
	// 如果有的话，就把这个对象加到house里
	houseInfo.Area = &tmparea
	/*--------------------------------------------------------------------*/
	qs = o.QueryTable("property")
	tmpProperty := models.Property{}
	err = qs.Filter("property", property).One(&tmpProperty)
	// 如果没有这样的房间，就往表里插入
	if err != nil {
		tmpProperty.Property = property
		_, err = o.Insert(&tmpProperty)
		if err != nil {
			fmt.Println("插入失败", err)
			o.Rollback()
		}
	}
	// 如果有的话，就把这个对象加到house里
	houseInfo.Property = &tmpProperty
	/*--------------------------------------------------------------------*/
	qs = o.QueryTable("house_type")
	tmpType := models.HouseType{}
	err = qs.Filter("type", Htype).One(&tmpType)
	// 如果没有这样的房间，就往表里插入
	if err != nil {
		tmpType.Type = Htype
		_, err = o.Insert(&tmpType)
		if err != nil {
			fmt.Println("插入失败", err)
			o.Rollback()
		}
	}
	// 如果有的话，就把这个对象加到house里
	houseInfo.HouseType = &tmpType
	/*--------------------------------------------------------------------*/
	qs = o.QueryTable("decoration")
	tmpDec := models.Decoration{}
	err = qs.Filter("decoration", decoration).One(&tmpDec)
	// 如果没有这样的房间，就往表里插入
	if err != nil {
		tmpDec.Decoration = decoration
		_, err = o.Insert(&tmpDec)
		if err != nil {
			fmt.Println("插入失败", err)
			o.Rollback()
		}
	}
	// 如果有的话，就把这个对象加到house里
	houseInfo.Decoration = &tmpDec
	/*--------------------------------------------------------------------*/
	_, err = o.Insert(&houseInfo)
	if err != nil {
		fmt.Println(err)
		o.Rollback()
	}
	o.Commit()
	c.Redirect("/houseInfo", 302)
}

func (c *MainController) DetailGet() {
	userId := c.GetSession("userId")
	if userId == nil {
		c.Redirect("/login", 302)
	}
	id := c.Input().Get("id")
	if id == "" {
		c.Redirect("/houseInfo", 302)
		return
	}
	fmt.Println(id)
	o := orm.NewOrm()
	qs := o.QueryTable("house")
	houseInfo := models.House{}
	err := qs.Filter("id", id).One(&houseInfo)
	if err != nil {
		fmt.Println(err)
	}

	fmt.Println(houseInfo.String())
	//err = qs.Filter("inter__name", houseInfo.Inter.Id).One(&houseInfo)
	//if err != nil {
	//	fmt.Println(err)
	//}
	//fmt.Println(houseInfo.String())
	c.Data["space"] = houseInfo.Space
	c.Data["price"] = houseInfo.Price
	c.Data["dir"] = houseInfo.Direction
	c.Data["floor"] = houseInfo.Floor
	c.Data["age"] = houseInfo.Age
	c.Data["emergency"] = houseInfo.Emergency

	inter := models.Inter{}
	qs = o.QueryTable("inter")
	qs.Filter("id", houseInfo.Inter.Id).One(&inter)
	c.Data["owner"] = inter.Name

	rooms := models.Rooms{}
	qs = o.QueryTable("rooms")
	qs.Filter("id", houseInfo.Rooms.Id).One(&rooms)
	c.Data["rooms"] = rooms.Rooms

	subway := models.Subway{}
	qs = o.QueryTable("subway")
	qs.Filter("id", houseInfo.Subway.Id).One(&subway)
	c.Data["station"] = subway.Station
	c.Data["line"] = subway.Line

	area := models.Area{}
	qs = o.QueryTable("area")
	qs.Filter("id", houseInfo.Area.Id).One(&area)
	c.Data["area"] = area.Area

	property := models.Property{}
	qs = o.QueryTable("property")
	qs.Filter("id", houseInfo.Property.Id).One(&property)
	c.Data["property"] = property.Property

	Type := models.HouseType{}
	qs = o.QueryTable("house_type")
	qs.Filter("id", houseInfo.HouseType.Id).One(&Type)
	c.Data["type"] = Type.Type

	decoration := models.Decoration{}
	qs = o.QueryTable("decoration")
	qs.Filter("id", houseInfo.Decoration.Id).One(&decoration)
	c.Data["decoration"] = decoration.Decoration

	c.Data["id"] = id

	fmt.Println("detail of a house: ", c.Data)

	c.TplName = "detail.html"
}

func (c *MainController) DetailPost() {
	requestBodyMap := make(map[string]string)
	UnmarshalErr := json.Unmarshal(c.Ctx.Input.RequestBody, &requestBodyMap)
	if UnmarshalErr != nil {
		fmt.Println(UnmarshalErr)
	}
	fmt.Println(requestBodyMap)
	o := orm.NewOrm()
	isInter := c.GetSession("isInter")
	if isInter == true {
		c.Redirect("/login", 302)
		return
	}
	id, err := strconv.Atoi(requestBodyMap["houseId"])
	userid := c.GetSession("userId")

	if requestBodyMap["type"] == "booking" {
		house := models.House{}
		qs := o.QueryTable("house")
		if err != nil {
			fmt.Println(err)
		}
		err = qs.Filter("id", id).One(&house)
		if err != nil {
			fmt.Println(err)
		}

		user := models.User{}
		qs = o.QueryTable("user")
		err = qs.Filter("id", userid).One(&user)
		if err != nil {
			fmt.Println(err)
		}

		app := models.Appointment{}
		qs = o.QueryTable("Appointment")
		err = qs.Filter("user_id", user.Id).Filter("house_id", house.Id).One(&app)
		if err == nil {
			// 有这个记录，那么跳过
			fmt.Println("已经有记录了")
			goto Finish
		}
		app.House = &house
		app.User = &user

		o.Begin()
		_, err = o.Insert(&app)
		if err != nil {
			fmt.Println(err)
			o.Rollback()
		}
		o.Commit()

	} else {
		qs := o.QueryTable("house")
		house := models.House{}
		err = qs.Filter("id", id).One(&house)
		if err != nil {
			fmt.Println("不存在house", err)
		}
		if v, ok := userid.(int); ok {
			qs = o.QueryTable("user")
			user := models.User{}
			qs.Filter("id", v).One(&user)

			m2m := o.QueryM2M(&house,"Favourites")
			ret := m2m.Exist(user)
			num,err := m2m.Count()
			fmt.Println("有",num,"条")
			if ret == false{
				// 没有互相收藏的记录
				_,err = m2m.Add(user)
				if err != nil {
					fmt.Println("更新失败", err)
				}
			}else{
				// 有
				goto Finish
			}
		} else {
			goto Finish
		}

	}
Finish:
	c.TplName = "detail.html"
}
