class Course:  # weight, price pairs
    def __init__(self, name,start, end):
        self.name = name
        self.start = start
        self.end = end

    def __str__(self):
        return self.name

def findMaxCourses(courses):
    maxCourses = []
    i = 0
    maxCourses.append(courses[i]) #first lesson is always taken
    for j in range(len(courses)):
        if courses[j].start >= courses[i].end: #if the previous lesson starts after current lesson
            maxCourses.append(courses[j])  #add it to list
            i = j #and update the previous lesson
    return maxCourses


courses = []
courses.append(Course('English',1,2))
courses.append(Course('Mathematics',3,4))
courses.append(Course('Physics',0,6))
courses.append(Course('Chemistry',5,7))
courses.append(Course('Biology',8,9))
courses.append(Course('Geography',5,9))

maxCourses = findMaxCourses(courses)

for i in maxCourses:
    print(i)