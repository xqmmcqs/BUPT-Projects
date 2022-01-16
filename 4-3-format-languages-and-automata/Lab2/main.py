def input_lang():
    global N, T, P, S
    str = input("请输入非终结符，以空格分隔：\n")
    for i in str.split(' '):
        N.add(i)

    str = input("请输入终结符，以空格分隔：\n")
    for i in str.split(' '):
        T.add(i)

    while True:
        str = input(
            "请输入一个生成式，以空格分隔每个符号，第一个符号为生成式的左端，以[empty]表示空串，以空行表示输入生成式结束：\n")
        if len(str) == 0:
            break
        state = str.split()
        for i in state:
            if i == "[empty]":
                continue
            if i not in N and i not in T:
                raise Exception("{} 不在符号集合内".format(i))
        if state[0] not in N:
            raise Exception("{} 不在非终结符集合内".format(state[0]))
        if state[0] in P:
            P[state[0]].append(tuple(state[1:]))
        else:
            P[state[0]] = [tuple(state[1:])]

    for key in P.keys():
        P[key] = list(set(P[key]))

    S = input("请输入起始符：\n")


def print_lang():
    global N, T, P, S
    print("N: {", end='')
    for i in N:
        print(i + ", ", end='')
    print("}")
    print("T: {", end='')
    for i in T:
        print(i + ", ", end='')
    print("}")
    print("P:")
    for key, value in P.items():
        print("\t" + key + " -> ", end='')
        for r in value:
            for i in r:
                print(i, end='')
            print(" | ", end='')
        print("")
    print("S: " + S)


def eliminate_epsilon():
    print("消去epsilon产生式")
    global N, T, P, S
    N0 = set()
    N1 = set()
    for key, value in P.items():
        for r in value:
            if len(r) == 1 and r[0] == "[empty]":
                N1.add(key)
                break

    while N0 != N1:
        N0 = N1.copy()
        for key, value in P.items():
            for r in value:
                in_N0 = True
                for i in r:
                    if i not in N0:
                        in_N0 = False
                        break
                if in_N0:
                    N1.add(key)
                    break

    P1 = dict()
    for key, value in P.items():
        P1[key] = []
        for r in value:
            total_in_N1 = 0
            for i in r:
                if i in N1:
                    total_in_N1 += 1

            for num in range(2 ** total_in_N1):
                new_r = []
                num_in_N1 = 0
                for i in r:
                    if i in N1:
                        if (num >> num_in_N1) & 0x1:
                            new_r.append(i)
                        num_in_N1 += 1
                    else:
                        new_r.append(i)
                if len(new_r) != 0 and new_r[0] != "[empty]":
                    P1[key].append(tuple(new_r))
        P1[key] = list(set(P1[key]))

    if S in N1:
        new_S = S + '1'
        while new_S in N or new_S in T:
            new_S += '1'
        P1[new_S] = [tuple(["[empty]"]), tuple(S)]
        S = new_S
        N.add(S)

    P = P1

def eliminate_single():
    print("消去单产生式")
    global N, T, P, S
    P1 = dict()
    for A in N:
        P1[A] = []
        N0 = {A}
        N1 = {A}
        for r in P[A]:
            if len(r) == 1 and r[0] in N:
                N1.add(r[0])
        while N0 != N1:
            N0 = N1.copy()
            for B in N0:
                for r in P[B]:
                    if len(r) == 1 and r[0] in N:
                        N1.add(r[0])
        for B in N0:
            for r in P[B]:
                if not (len(r) == 1 and r[0] in N):
                    P1[A].append(r)
        P1[A] = list(set(P1[A]))

    P = P1


def eliminate_useless():
    print("消去无用符号")
    global N, T, P, S
    N0 = set()
    N1 = set()
    for key, value in P.items():
        for r in value:
            in_T = True
            for i in r:
                if i not in T:
                    in_T = False
                    break
            if in_T:
                N1.add(key)
                break

    while N0 != N1:
        N0 = N1.copy()
        for key, value in P.items():
            for r in value:
                in_N0 = True
                for i in r:
                    if i not in N0 and i not in T:
                        in_N0 = False
                        break
                if in_N0:
                    N1.add(key)
                    break
    N = N1
    
    P1 = dict()
    for key, value in P.items():
        if key not in N:
            continue
        P1[key] = []
        for r in value:
            in_N = True
            for i in r:
                if i != "[empty]" and i not in N and i not in T:
                    in_N = False
                    break
            if in_N:
                P1[key].append(r)
    P = P1

    N0 = {S}
    N1 = {S}
    for r in P[S]:
        for i in r:
            N1.add(i)
    while N0 != N1:
        N0 = N1.copy()
        for A in N0:
            if A not in N:
                continue
            for r in P[A]:
                for i in r:
                    N1.add(i)
    N = N.intersection(N1)
    T = T.intersection(N1)
    
    P1 = dict()
    for key, value in P.items():
        if key not in N:
            continue
        P1[key] = []
        for r in value:
            in_N = True
            for i in r:
                if i != "[empty]" and i not in N and i not in T:
                    in_N = False
                    break
            if in_N:
                P1[key].append(r)
    P = P1


N = set()
T = set()
P = dict()
S = ""

if __name__ == "__main__":
    input_lang()
    print_lang()
    eliminate_epsilon()
    print_lang()
    eliminate_single()
    print_lang()
    eliminate_useless()
    print_lang()
