def fibonacci_re(n):
    if n < 0:
        return 0
    elif n == 1:
        return 1
    else:
        return fibonacci_re(n-1) + fibonacci_re(n-2)
