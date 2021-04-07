package main

import (
	"database/sql"
	"fmt"
	"log"

	//"time"
	_ "github.com/go-sql-driver/mysql"
)
var Db *sql.DB

func main() {
	initDb()
	query()

	defer Db.Close()

}

func initDb(){
	db, err := sql.Open("mysql", "root:000448664@/test?charset=utf8")
	if err != nil {
		log.Panicln(err)
	}
	Db = db
}
func insert(){
	//插入数据
	stmt, err := Db.Prepare("INSERT INTO userinfo SET username=?,department=?,created=?")
	checkErr(err)

	res, err := stmt.Exec("ast3axie2", "研发部3门", "2012-11-09")
	checkErr(err)

	id, err := res.LastInsertId()
	checkErr(err)

	fmt.Println("id: ",id)
}
func update(id int)  {

	//更新数据
	stmt, err := Db.Prepare("update userinfo set username=? where uid=?")
	checkErr(err)

	res, err := stmt.Exec("astaxieupdate", id)
	checkErr(err)

	affect, err := res.RowsAffected()
	checkErr(err)

	fmt.Println(affect)
}
func query()  {
	//查询数据
	rows, err := Db.Query("SELECT * FROM userinfo")
	checkErr(err)
	fmt.Print("uid\t\t")
	fmt.Print("username\t\t")
	fmt.Print("dep\t\t")
	fmt.Print("create\t\n")
	for rows.Next() {
		var uid int
		var username string
		var department string
		var created string
		err = rows.Scan(&uid, &username, &department, &created)
		checkErr(err)
		fmt.Print(uid,"\t\t")
		fmt.Print(username,"\t\t")
		fmt.Print(department,"\t\t")
		fmt.Print(created,"\t\t")
		fmt.Println()
	}
}
func delete(id int)  {
	//删除数据
	stmt, err := Db.Prepare("delete from userinfo where uid=?")
	checkErr(err)


	res, err := stmt.Exec(id)
	checkErr(err)

	affect, err := res.RowsAffected()
	checkErr(err)

	fmt.Println(affect)
}




func checkErr(err error) {
	if err != nil {
		panic(err)
	}
}