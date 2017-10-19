# Contributing guidelines

To contribute, please follow this process:

## 1. Sign CLA agreement
Please sign it via GitHub credentials: 
[Developer Certificate of Origin Version 1.1](https://cla-assistant.io/hyperledger/iroha)

## 2. Read Iroha working agreement

[Iroha working agreement](https://github.com/hyperledger/iroha/wiki/Iroha-working-agreement)

## 3. Talk to us
Before writing any code, please discuss your intention with us, so that we can help you with your solution. It can also help to reduce situations with duplicated effort.
 
Do anything that is more convenient to you:
* Join [Hyperledger RocketChat](https://chat.hyperledger.org/) #iroha channel
* Open new issue in GitHub or discuss existing issue
* Join telegram [Iroha chat](https://t.me/joinchat/AgzrTUCZ6efra09wz35oww) 
* Use mailing list to discuss hyperledger-iroha@lists.hyperledger.org
* Communicate in [Gitter chat](https://gitter.im/hyperledger-iroha/Lobby) with our development community 

## 4. Check codestyle

Take a look briefly at [CppCoreGuidelines](https://github.com/isocpp/CppCoreGuidelines) and use [linter](https://github.com/hyperledger/iroha/blob/master/.clang-format) to check your code before creating pull request.

## 5. Get acquainted with design

As you are going to discuss your change in prior, we will help you to understand design of the system. Please, check architecture section to understand responsibilities of components and interfaces.

## 6. Test your code

Please, follow test policy for the code you write. New code should be covered by at least 80% (see coverage reports in pull requests). 

## 7. Create pull request

Follow gitflow approach to create a branch for your code:
* feature/whatever-feature-you-implement, create a branch from develop
* fix/whatever-you-fix-in-develop
* hotfix/whatever-you-fix-in-master

Follow the flow in Iroha working agreement.

Thank you for your interest to Iroha project!
