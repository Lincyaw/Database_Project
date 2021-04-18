package models

import (
	"bytes"
	"encoding/json"
	"fmt"
	"github.com/astaxie/beego/orm"
	_ "github.com/go-sql-driver/mysql"
	"time"
)

// model 里存放表的设计
type User struct {
	Id          int
	Name        string         `orm:"index"`
	Pwd         string         `orm:"size(20)"`
	PhoneNum    string         `orm:"index;unique;null"`
	Mail        string         `orm:"index;unique;null"`
	Wechat      string         `orm:"unique;null"`
	Avatar      string         `orm:"size(256);null"`
	Appointments []*Appointment `orm:"reverse(many)"`
	Favourites  []*House       `orm:"reverse(many)"`
}

type Inter struct {
	Id       int
	Name     string   `orm:"index"`
	Pwd      string   `orm:"size(20)"`
	PhoneNum string   `orm:"index;unique;null"`
	Mail     string   `orm:"index;unique;null"`
	Wechat   string   `orm:"unique;null"`
	Avatar   string   `orm:"size(256)"`
	Houses   []*House `orm:"reverse(many)"`
}

type ChatLog struct {
	Id    int
	User  *User     `orm:"rel(one)"`
	Inter *Inter    `orm:"rel(one)"`
	Time  time.Time `orm:"auto_now_add;type(datetime)"`
	Data  string    `orm:"size(1024)"`
}

type Appointment struct {
	Id    int
	User  *User     `orm:"rel(fk)"`
	House *House    `orm:"rel(fk)"`
	Time  time.Time `orm:"auto_now_add;type(datetime)"`
}

//type Favourite struct {
//	Id    int
//	User  *User     `orm:"rel(fk)"`
//	House *House    `orm:"rel(fk)"`
//	Time  time.Time `orm:"auto_now_add;type(datetime)"`
//}

func (conf *House) String() string {
	b, err := json.Marshal(*conf)
	if err != nil {
		return fmt.Sprintf("%+v", *conf)
	}
	var out bytes.Buffer
	err = json.Indent(&out, b, "", "    ")
	if err != nil {
		return fmt.Sprintf("%+v", *conf)
	}
	return out.String()
}

type House struct {
	Id           int
	Space        int `orm:"null"`
	Price        int
	Direction    int            `orm:"null"`
	Floor        int            `orm:"null"`
	Age          int            `orm:"null"`
	Emergency    int            `orm:"null"`
	Subway       *Subway        `orm:"rel(fk)"`
	Area         *Area          `orm:"rel(fk)"`
	Property     *Property      `orm:"rel(fk)"`
	Rooms        *Rooms         `orm:"rel(fk)"`
	HouseType    *HouseType     `orm:"rel(fk)"`
	Decoration   *Decoration    `orm:"rel(fk)"`
	Inter        *Inter         `orm:"rel(fk)"`
	Favourites   []*User         `orm:"rel(m2m)"`
	Appointments []*Appointment `orm:"reverse(many)"`
}

type Subway struct {
	Id      int
	Line    string   `orm:"index"`
	Station string   `orm:"index"`
	Houses  []*House `orm:"reverse(many)"`
}

type Area struct {
	Id     int
	Area   string   `orm:"index"`
	Houses []*House `orm:"reverse(many)"`
}
type Property struct {
	Id       int
	Property string   `orm:"index"`
	Houses   []*House `orm:"reverse(many)"`
}

type Rooms struct {
	Id     int
	Rooms  int      `orm:"index"`
	Houses []*House `orm:"reverse(many)"`
}

type HouseType struct {
	Id     int
	Type   string   `orm:"index"`
	Houses []*House `orm:"reverse(many)"`
}

type Decoration struct {
	Id         int
	Decoration string   `orm:"index"`
	Houses     []*House `orm:"reverse(many)"`
}

func init() {
	orm.RegisterDataBase("default", "mysql", "root:000448664@tcp(127.0.0.1:3306)/test?charset=utf8")
	// 映射model数据
	orm.RegisterModel(new(User), new(Inter), new(ChatLog), new(Appointment), new(House), new(Subway),
		new(Area), new(Property), new(Rooms), new(HouseType), new(Decoration))
	// 别名， 是否强制更新， 是否可变
	orm.RunSyncdb("default", false, true)
}
