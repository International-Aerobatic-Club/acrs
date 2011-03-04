ALTER TABLE contest ADD regOpen date not null;
update contest set regOpen = regDeadline;
update contest set regOpen = now() where now() < regDeadline;
