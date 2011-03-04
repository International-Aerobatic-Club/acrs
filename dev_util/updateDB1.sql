ALTER TABLE ctst_cat ADD hasFourMinute enum('y', 'n') not null default 'n';
ALTER TABLE ctst_cat ADD fourMinRegAmt smallint unsigned;
create table if not exists slot_restriction(
       sessID int unsigned not null,
       slotIndex smallint not null,
       restrictionType enum('class', 'category') default 'class',
       class enum('power', 'glider', 'other') default 'other',
       catID int unsigned,
       unique(sessID, slotIndex),
       key(sessID));
