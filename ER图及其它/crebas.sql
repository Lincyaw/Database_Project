/*==============================================================*/
/* DBMS name:      MySQL 5.0                                    */
/* Created on:     2021/4/4 17:34:47                            */
/*==============================================================*/


drop  
table  if   exists   user_login;

drop  
table  if   exists   inter_login;

drop table if exists appointment;

drop table if exists chatlog;

drop table if exists house;

drop table if exists house_area;

drop table if exists house_dec;

drop table if exists house_property;

drop table if exists house_rooms;

drop table if exists house_subway;

drop table if exists house_type;

drop table if exists intermediary;

drop table if exists user;

drop table if exists viewd_time;

/*==============================================================*/
/* Table: appointment                                           */
/*==============================================================*/
create table appointment
(
   user_id              int not null,
   house_id             int not null,
   time                 datetime,
   primary key (user_id, house_id)
);

/*==============================================================*/
/* Table: chatlog                                               */
/*==============================================================*/
create table chatlog
(
   user_id              int not null,
   zj_id                int not null,
   time                 datetime,
   content              varchar(1024),
   primary key (user_id, zj_id)
);

/*==============================================================*/
/* Table: house                                                 */
/*==============================================================*/
create table house
(
   house_id             int not null,
   user_id              int,
   use_user_id          int,
   sub_id               int not null,
   rooms_id             int not null,
   property_id          int not null,
   hou_house_id         int not null,
   zj_id                int,
   house_price          int,
   house_space          int,
   house_dir            int,
   house_floor          int,
   house_age            int,
   house_emr            int,
   primary key (house_id)
);

/*==============================================================*/
/* Table: house_area                                            */
/*==============================================================*/
create table house_area
(
   area_id              int not null,
   house_id             int,
   house_area           varchar(10),
   primary key (area_id)
);

/*==============================================================*/
/* Table: house_dec                                             */
/*==============================================================*/
create table house_dec
(
   house_id             int not null,
   house_decoration     varchar(10),
   primary key (house_id)
);

/*==============================================================*/
/* Table: house_property                                        */
/*==============================================================*/
create table house_property
(
   property_id          int not null,
   house_property       varchar(10),
   primary key (property_id)
);

/*==============================================================*/
/* Table: house_rooms                                           */
/*==============================================================*/
create table house_rooms
(
   rooms_id             int not null,
   house_rooms          varchar(10),
   primary key (rooms_id)
);

/*==============================================================*/
/* Table: house_subway                                          */
/*==============================================================*/
create table house_subway
(
   sub_id               int not null,
   house_subway         varchar(10),
   house_station        varchar(10),
   primary key (sub_id)
);

/*==============================================================*/
/* Table: house_type                                            */
/*==============================================================*/
create table house_type
(
   house_id             int not null,
   house_type           varchar(10),
   primary key (house_id)
);

/*==============================================================*/
/* Table: intermediary                                          */
/*==============================================================*/
create table intermediary
(
   zj_id                int not null,
   nickname             char(10),
   mail                 text,
   phone                char(11),
   passwd               char(20),
   wechat               char(10),
   avatar               longblob,
   primary key (zj_id)
);

/*==============================================================*/
/* Table: user                                                  */
/*==============================================================*/
create table user
(
   user_nickname        char(10),
   user_id              int not null,
   user_mail            text,
   user_phone           int not null,
   user_passwd          char(20),
   user_wechat          char(10),
   user_avatar          longblob,
   primary key (user_id)
);

/*==============================================================*/
/* Table: viewd_time                                            */
/*==============================================================*/
create table viewd_time
(
   user_id              int not null,
   house_id             int not null,
   time                 datetime,
   primary key (user_id, house_id)
);

/*==============================================================*/
/* View: inter_login                                            */
/*==============================================================*/
create  VIEW      inter_login
  as
select mail,passwd
from user;

/*==============================================================*/
/* View: user_login                                             */
/*==============================================================*/
create  VIEW      user_login
  as
select user_mail,user_passwd
from user;

alter table appointment add constraint FK_appointment foreign key (house_id)
      references house (house_id) on delete restrict on update restrict;

alter table appointment add constraint FK_appointment2 foreign key (user_id)
      references user (user_id) on delete restrict on update restrict;

alter table chatlog add constraint FK_chatlog foreign key (zj_id)
      references intermediary (zj_id) on delete restrict on update restrict;

alter table chatlog add constraint FK_chatlog2 foreign key (user_id)
      references user (user_id) on delete restrict on update restrict;

alter table house add constraint FK_Relationship_1 foreign key (user_id)
      references user (user_id) on delete restrict on update restrict;

alter table house add constraint FK_Relationship_10 foreign key (zj_id)
      references intermediary (zj_id) on delete restrict on update restrict;

alter table house add constraint FK_Relationship_3 foreign key (use_user_id)
      references user (user_id) on delete restrict on update restrict;

alter table house add constraint FK_Relationship_5 foreign key (sub_id)
      references house_subway (sub_id) on delete restrict on update restrict;

alter table house add constraint FK_Relationship_6 foreign key (rooms_id)
      references house_rooms (rooms_id) on delete restrict on update restrict;

alter table house add constraint FK_Relationship_7 foreign key (house_id)
      references house_type (house_id) on delete restrict on update restrict;

alter table house add constraint FK_Relationship_8 foreign key (property_id)
      references house_property (property_id) on delete restrict on update restrict;

alter table house add constraint FK_Relationship_9 foreign key (hou_house_id)
      references house_dec (house_id) on delete restrict on update restrict;

alter table house_area add constraint FK_Relationship_4 foreign key (house_id)
      references house (house_id) on delete restrict on update restrict;

alter table viewd_time add constraint FK_viewd_time foreign key (house_id)
      references house (house_id) on delete restrict on update restrict;

alter table viewd_time add constraint FK_viewd_time2 foreign key (user_id)
      references user (user_id) on delete restrict on update restrict;

