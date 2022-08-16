using namespace std;

class queue {
    public:
        queue();
        void enqueue(int);
        int dequeue();
        int peek();
        int size();
        bool isEmpty();
    private:
        int arr[128];
        int front;
        int rear;
        int capacity;
        int size_;
};

