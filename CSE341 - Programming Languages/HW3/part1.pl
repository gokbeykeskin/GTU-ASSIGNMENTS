%knowledge base


/*
Courses: 101,102,241,341,343
Rooms: Z06,Z11,Z23
Instructors: YG,YSA,MG,HK
Students: student1,student2...
*/

%knowledge base

room(z06). %in the assign room case, there was a conflict between instructors and
room(z11). %rooms so I had the specify the rooms.
room(z23).


time(101,10).
time(102,13).
time(241,10).
time(341,10).
time(343,14).

% I used place1 instead of place because place is a reserved keyword of
% the SWI prolog
place1(101,z06).
place1(102,z11).
place1(241,z23).
place1(341,z06).
place1(343,z23).

needs(101,projector).
needs(102,smart_board).
needs(241,smart_board).
needs(241,projector).

needs2(yg,projector).
needs2(ysa,smart_board).
needs2(mg,projector).
needs2(mg,smart_board).


has(z23,projector).
has(z23,smart_board).
has(z06,projector).
has(z11,smart_board).

handicapped(student1).
handicapped(student5).

handicap_friendly(z23).
handicap_friendly(z06).

capacity(z06,50).
capacity(z11,70).
capacity(z23,90).

capacity(101,80).
capacity(102,70).
capacity(241,60).
capacity(341,40).
capacity(343,50).

teaches(yg,102).
teaches(yg,341).
teaches(ysa,241).
teaches(mg,101).
teaches(hk,343).


%rules

conflict(X,Y):- %X and Y are courses
    X\=Y,
    (place1(X,A),place1(Y,A),
    time(X,B), time(Y,B)).

assignRoom(C,R):-
    room(R),
    capacity(C, A), capacity(R, B), A=<B, %if the space in room is sufficient for the course
    forall(needs(C, X), has(R, X)), %a room might need an item
    teaches(I,C),
    forall(needs2(I,X), has(R, X)). %instructor might need an item

enroll(S,C):-
    \+ (handicapped(S));(handicapped(S),place1(C,X),handicap_friendly(X)).
    %S can be enrolled to C if he/she is not handicapped or the room which course C takes place is handicap friendly.

















































