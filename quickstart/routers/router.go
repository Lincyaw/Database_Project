package routers

import (
	"github.com/astaxie/beego"
	"quickstart/controllers"
)

func init() {
	beego.Router("/register", &controllers.MainController{}, "get:ShowRegister;post:HandleRegister")
	// 当实现了自定义的get方法后，请求将不会访问默认方法
	beego.Router("/login", &controllers.MainController{}, "get:ShowLogin;post:HandleLogin")
	beego.Router("/", &controllers.MainController{})
	beego.Router("/about", &controllers.MainController{}, "get:AboutGet")
	beego.Router("/exit", &controllers.MainController{}, "get:ExitGet")
	beego.Router("/user", &controllers.MainController{}, "get:UserInfoGet;post:UserInfoPost")
	beego.Router("/sendHouse", &controllers.MainController{}, "get:SendGet;post:SendPost")


	beego.Router("/houseInfo", &controllers.HouseController{}, "get:HouseGet")
	beego.Router("/detail", &controllers.MainController{}, "get:DetailGet;post:DetailPost")

}
