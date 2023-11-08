# AWS IoT Core Thing 채널 쉐도우 정의 (v1.2)

## 변경 사항 (v1.2)

> * 추후 사용할 동작 모드 필드 추가

## 변경 사항 (v1.1)

> * value -> state: keyword 충돌 예방

## 채널 Shadow 이름

> * farmChannels

## 채널 Shadow 형식

```json
{
  "state": {
    "desired": {
      "{actuator|sensor}-{채널 번호}": {
        "model": "{모델 번호: 양의 정수}",
        "state": "{상태 값: 실수}",
        "mode": "{동작 모드: 실수}"
      }
    },
    "reported": {
      "{actuator|sensor}-{채널 번호}": {
        "model": "{모델 번호: 양의 정수}",
        "state": "{상태 값: 실수}",
        "mode": "{동작 모드: 실수}"
      }
    }
  }
}
```

> 채널 타입 종류
> > * sensor: 값을 측정할 수 있는 채널, 예시) 온도 센서, 습도 센서, Co2 센서 등등
> > * actuator: 입력을 받아 동작할 수 있는 채널, 예시) 모터(3-방향 릴레이), 릴레이(2-방향 릴레이) 등등
> >
> 채널 번호
> > * sensor: 0 ~ 20
> > * actuator: 0 ~ 100
> >
> 모델 번호
> > * 모터(3 방향 릴레이): 1
> > * 릴레이(2 방향 릴레이): 2
> > * 온도 센서: 30
> > * 습도 센서: 31
> > * Co2 센서: 32
> > * EC 센서: 33
> >
> 상태 값
> > * 모터(3 방향 릴레이): -1, 0, 1
> > * 릴레이(2 방향 릴레이): 0, 1
> > * 온도 센서: -100 ~ 100 (단위: °C)
> > * 습도 센서: 0 ~ 100 (단위: %)
> > * Co2 센서: 0 ~ 100 (단위: %)
> > * EC 센서: 0 ~ 14 (단위: ph)
> >
> 동작 모드 값
> > * 수동: 0
> > * 반자동: 1
> > * 원격: 2

## 모듈 명령 송신 쉐도우 형식 - API 서버측

```json
{
  "state": {
    "desired": {
      "actuator-{채널 번호}": {
        "state": "{상태 값: 실수}"
      }
    }
  }
}
```

> * 주의 사항: 동시에 여러 채널에 대한 명령을 전달할 수 있음.

## 모듈 명령 수신 쉐도우 형식 - 사물측

```json
{
  "state": {
    "reported": {
      "actuator-{채널 번호}": {
        "state": "{상태 값: 실수}"
      }
    }
  }
}
```

> * 주의 사항: state 필드와 level 필드가 동시에 전송 안할경우, 값을 무시함.
> > 배포 MQTT 토픽
> > * $aws/things/{thingName}/shadow/name/farmChannels/update

## 모듈 모델 변경 송신 쉐도우 형식 - 사물

```json
{
  "state": {
    "reported": {
      "{actuator|sensor}-{채널 번호}": {
        "model": "{모델 번호: 양의 정수}"
      }
    }
  }
}
```

> > 구독 MQTT 토픽
> > * $aws/things/{thingName}/shadow/name/farmChannels/update/delta
> * 채널 모델이 변경된 채널만 송신

## 모듈 플러그 쉐도우 형식 - 사물측

```json
{
  "state": {
    "reported": {
      "{actuator|sensor}-{채널 번호}": {
        "model": "{모델 번호: 양의 정수}",
        "state": "{상태 값: 실수}",
        "mode": "{동작 모드: 실수}"
      }
    }
  }
}
```

> * 플러그된 채널만 송신. (변경 안된 채널은 송신 X)
> * model 또는 value 필드가 null 또는 없을시 언플러그 처리함.
> > 배포 MQTT 토픽
> > * $aws/things/{thingName}/shadow/name/farmChannels/update

## 모듈 언플러그 쉐도우 형식 - 사물측

```json
{
  "state": {
    "reported": {
      "{actuator|sensor}-{채널 번호}": null
    }
  }
}
```

> * 언플러그된 채널만 송신. (변경 안된 채널은 송신 X)
> > 배포 MQTT 토픽
> > * $aws/things/{thingName}/shadow/name/farmChannels/update

## 센서값 송신 쉐도우 형식 - 사물측

```json
{
  "state": {
    "reported": {
      "sensor-{채널 번호}": {
        "state": "{상태 값: 실수}"
      }
    }
  }
}
```

> * 모든 센서 값 송신. (액추레이터 채널은 송신 X )
> > 배포 MQTT 토픽
> > * $aws/things/{thingName}/shadow/name/farmChannels/update

## 채널 쉐도우 요청 방법

> 1) $aws/things/{thingName}/shadow/name/farmChannels/get/accepted 를 구독 한다.
> 2) $aws/things/{thingName}/shadow/name/farmChannels/get를 배포 한다.