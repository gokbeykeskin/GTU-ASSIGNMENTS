%knowledge base
flight(canakkale,erzincan,6).
flight(erzincan,antalya,3).
flight(antalya,izmir,2).
flight(antalya,diyarbakir,4).
flight(istanbul,izmir,2).
flight(ankara,izmir,6).
flight(ankara,istanbul,1).
flight(rize,istanbul,4).
flight(ankara,rize,5).
flight(van,ankara,4).
flight(diyarbakir,ankara,8).
flight(gaziantep,van,3).


% rules
%
% It is a non-directed graph so every possible flight is bidirectional:
neighbour_route(X, Y, C):-
    flight(X, Y, C).
neighbour_route(X, Y, C):-
    flight(Y, X, C).

route(X, Y, C):- %wrapper relation for search_route relation
    search_route(X, Y, C ,[]).

search_route(X, Y, C, Visited):-
    %There exists a path between X and Y which costs C if:
    neighbour_route(X,Y,C) %X and Y are direct neighbours and route costs C
    ; % or
    (
            neighbour_route(X, Z, A)%X and Z are neighbours and route between them costs A
            ,%and
            \+ (member(Z, Visited))%Z is not a visited node
            ,%and
            search_route(Z, Y,B, [X | Visited])%there exists a path between Z and Y which costs B
            ,%and
            C is A+B,%C is A+B
            %and
            X\=Y%X is not Y (to avoid circular paths.
    ).

